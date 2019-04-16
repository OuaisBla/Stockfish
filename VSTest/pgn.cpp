
#include "pch.h"
#include "pgn.h"
#include "movegen.h"
#include <istream>
#include <string>
#include <list>
#include <utility>
#include <algorithm> 
#include <cctype>
#include <locale>


const char* PGNParser::StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

namespace {

	const std::string PieceToChar(" PNBRQK  pnbrqk");
	const std::string PieceToSAN(" PNBRQK  PNBRQK");

}

template<PieceType Pt, bool Checks>
ExtMove* generate_moves(const Position& pos, ExtMove* moveList, Color us, Bitboard target);

template<Color Us, GenType Type>
ExtMove* generate_pawn_moves(const Position& pos, ExtMove* moveList, Bitboard target);

template<GenType Type, Direction D>
ExtMove* make_promotions(ExtMove* moveList, Square to, Square ksq);


// trim from start (in place)
static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
    return !std::isspace(ch) && ch != ',';
  }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
    return !std::isspace(ch) && ch != ',';
  }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
  ltrim(s);
  rtrim(s);
}


PGNParser::PGNParser(std::istream& is)
  : is_(is) {
}

bool is_result(const std::string& result) {
  return 
    result == "1-0" || 
    result == "0-1" || 
    result == "1/2-1/2";
}

int parse_result(const std::string& result) {
  if (result == "1-0") {
    return 1;
  } else if (result == "0-1") {
    return -1;
  } else if (result == "1/2-1/2") {
    return 0;
  }

  throw std::runtime_error("Unknown result: " + result);
}

std::unique_ptr<PGNGame> PGNParser::parse() {
  // Skip all the PGN headers
  std::string s;
  std::string result;
  const std::string kResultToken = "[Result \"";
  for (;;) {
    getline(is_, s);
    if (s.substr(0, kResultToken.size()) == kResultToken) {
      result = s.substr(kResultToken.size(), s.size() - kResultToken.size() - 2);
    }
    if (s.empty()) {
      break;
    }
  }

  if (is_.eof()) {
    return nullptr;
  }

  std::unique_ptr<PGNGame> game(new PGNGame);
  game->bh.set(StartFEN);

  game->result = parse_result(result);

  std::list<std::string> stats;

  // Read in the moves
  for (int i = 0;;) {

    is_ >> s;

    if (!is_.good() || s.empty() || s == "[Event" || is_result(s)) {
      break;
    }

    // Skip simple comments
    if ( (s.front() == '{' || s.front() == '[') && (s.back() == '}' || s.back() == ']') ) {
      continue;
    }

    // Parse comments with stats
    if (s.front() == '{' || s.front() == '[' ) {

      s = s.substr(1, s.length());
      
      stats.push_back(s);

      while (s.back() != '}' && s.back() != ']') {
        is_ >> s;

        trim(s);

        stats.push_back(s);
      }

      s = s.substr(0, s.length()-1);
      stats.push_back(s);

      continue;
    }

    // Skip the move numbers
    if (s.back() == '.') {
      continue;
    }

    // Drop annotations
    if (s.back() == '!' || s.back() == '?') {
      size_t len = s.length();
      auto backIt = s.end();
      backIt--;
      while (*backIt == '!' || *backIt == '?') {
        backIt--;
        len--;
      }
      s = s.substr(0, len);
    }

    ++i;
    Move m = game->bh.san_to_move(game->bh.cur(), s);
    if (m == MOVE_NONE) {
      throw std::runtime_error("Unable to parse pgn move " + s);
    }

    MoveStats movestats = MoveStatsParser::Parse(stats);

    game->bh.do_move(m, movestats);

    stats.clear();
  }

  // Read the empty line after the game
  getline(is_, s);
  getline(is_, s);

  return game;
}


std::string BoardHistory::move_to_san(Position const& pos, Move m) const {
	Square from = from_sq(m);
	Square to = to_sq(m);
	Piece pc = pos.moved_piece(m);

	std::string result = "";
	if (type_of(m) == CASTLING) {
		result = file_of(to) > FILE_E ? "O-O" : "O-O-O";
	}
	else {
		switch (type_of(pc)) {
		case PAWN: break;
		case KNIGHT: result += "N"; break;
		case BISHOP: result += "B"; break;
		case ROOK: result += "R"; break;
		case QUEEN: result += "Q"; break;
		case KING: result += "K"; break;
		default: break;
		}

		MoveList<LEGAL> moves(pos);
		bool dupe = false, rank_diff = true, file_diff = true;
		for (auto m2 : moves) {
			if (from_sq(m2) != from && to_sq(m2) == to && type_of(pc) == type_of(pos.moved_piece(m2))) {
				dupe = true;
				if (file_of(from) == file_of(from_sq(m2))) file_diff = false;
				if (rank_of(from) == rank_of(from_sq(m2))) rank_diff = false;
			}
		}
		char file = "abcdefgh"[file_of(from)];
		char rank = '1' + rank_of(from);
		if (dupe) {
			if (file_diff) {
				result += file;
			}
			else if (rank_diff) {
				result += rank;
			}
			else {
				result += file;
				result += rank;
			}
		}
		else if (type_of(pc) == PAWN && (pos.piece_on(to) != NO_PIECE || type_of(m) == ENPASSANT)) {
			result += file;
		}

		if (pos.piece_on(to) != NO_PIECE || type_of(m) == ENPASSANT) {
			result += "x";
		}

		result += "abcdefgh"[file_of(to)];
		result += '1' + rank_of(to);
	}
	if (type_of(m) == PROMOTION) {
		switch (promotion_type(m)) {
		case KNIGHT: result += "=N"; break;
		case BISHOP: result += "=B"; break;
		case ROOK: result += "=R"; break;
		case QUEEN: result += "=Q"; break;
		default: break;
		}
	}
	if (pos.gives_check(m)) {
		result += "+";
	}
	return result;
}

/// Position::move_is_san() takes a pseudo-legal Move and a san as input and
/// returns true if moves are equivalent.
template<bool Strict>
bool BoardHistory::move_is_san(Position const& pos, Move m, const char* ref) const {

	assert(m != MOVE_NONE);

	Bitboard others, b;
	char buf[8], *san = buf;
	Square from = from_sq(m);
	Square to = to_sq(m);
	Piece pc = pos.piece_on(from);
	PieceType pt = type_of(pc);

	buf[2] = '\0'; // Init to fast compare later on

	if (type_of(m) == CASTLING)
	{
		int cmp, last = to > from ? 3 : 5;

		if (ref[0] == 'O')
			cmp = to > from ? strncmp(ref, "O-O", 3) : strncmp(ref, "O-O-O", 5);
		else if (ref[0] == '0')
			cmp = to > from ? strncmp(ref, "0-0", 3) : strncmp(ref, "0-0-0", 5);
		else if (ref[0] == 'o')
			cmp = to > from ? strncmp(ref, "o-o", 3) : strncmp(ref, "o-o-o", 5);
		else
			cmp = 1;

		return !cmp && (ref[last] == '\0' || ref[last] == '+' || ref[last] == '#');
	}

	if (pt != PAWN)
	{
		*san++ = PieceToSAN[pt];

		// A disambiguation occurs if we have more then one piece of type 'pt'
		// that can reach 'to' with a legal move.
		others = b = (pos.attacks_from(type_of(pc), to) & pos.pieces(pos.side_to_move(), pt)) ^ from;

		while (Strict && b)
		{
			Square s = pop_lsb(&b);
			if (!pos.legal(make_move(s, to)))
				others ^= s;
		}

		if (!others)
		{ /* Disambiguation is not needed */
		}

		else if (!(others & file_bb(from))
			&& (Strict || (ref[1] > '8'))) // Check for wrong row disambiguation
			*san++ = char('a' + file_of(from));

		else if (!(others & rank_bb(from)))
			*san++ = char('1' + rank_of(from));

		else
		{
			*san++ = char('a' + file_of(from));
			*san++ = char('1' + rank_of(from));
		}

		if (pos.capture(m) && (Strict || strchr(ref, 'x')))
			*san++ = 'x';

		// Add also if not a capture but 'x' is in ref
		else if (!Strict && strchr(ref, 'x'))
			*san++ = 'x';
	}
	else if (pos.capture(m))
	{
		*san++ = char('a' + file_of(from));

		if (Strict || strchr(ref, 'x'))
			*san++ = 'x';
	}

	*san++ = char('a' + file_of(to));
	*san++ = char('1' + rank_of(to));

	if (type_of(m) == PROMOTION)
	{
		if (Strict) // Sometime promotion move misses the '='
			*san++ = '=';

		*san++ = PieceToSAN[promotion_type(m)];
	}

	if (buf[1] != ref[1]
		|| buf[0] != ref[0])
		return false;

	if (san - buf > 2 && buf[2] != ref[2])
		return false;

	if (!ref[2] || !ref[3]) // Quiet move both pawn and piece: e4, Nf3
		return true;

	// Be forgivng if the move is missing check annotation
	return !strncmp(ref + 3, buf + 3, san - buf - 3);
}

template<CastlingRight Cr, bool Checks>
ExtMove* generate_castling(const Position& pos, ExtMove* moveList, Color us) {

  static const bool KingSide = (Cr == WHITE_OO || Cr == BLACK_OO);

  if (pos.castling_impeded(Cr) || !pos.can_castle(Cr))
    return moveList;

  // After castling, the rook and king final positions are the same in Chess960
  // as they would be in standard chess.
  Square kfrom = pos.square<KING>(us);
  Square rfrom = pos.castling_rook_square(Cr);
  Square kto = relative_square(us, KingSide ? SQ_G1 : SQ_C1);
  Bitboard enemies = pos.pieces(~us);

  assert(!pos.checkers());

  const Direction K = KingSide ? WEST : EAST;

  for (Square s = kto; s != kfrom; s += K)
    if (pos.attackers_to(s) & enemies)
      return moveList;

  // Because we generate only legal castling moves we need to verify that
  // when moving the castling rook we do not discover some hidden checker.
  // For instance an enemy queen in SQ_A1 when castling rook is in SQ_B1.

  Move m = make<CASTLING>(kfrom, rfrom);

  if (Checks && !pos.gives_check(m))
    return moveList;

  *moveList++ = m;
  return moveList;
}

template<Color Us, GenType Type, bool Checks, bool WithCastle = true>
ExtMove* generate_king_moves(const Position& pos, ExtMove* moveList, Bitboard target) {

  if (Type != QUIET_CHECKS && Type != EVASIONS)
  {
    Square ksq = pos.square<KING>(Us);
    Bitboard b = pos.attacks_from<KING>(ksq) & target;
    while (b)
      *moveList++ = make_move(ksq, pop_lsb(&b));
  }

  if (WithCastle && Type != CAPTURES && Type != EVASIONS && pos.can_castle( CastlingRight( pos.castling_rights(Us) ) ))
  {
    moveList = generate_castling<MakeCastling<Us, KING_SIDE>::right, Checks>(pos, moveList, Us);
    moveList = generate_castling<MakeCastling<Us, QUEEN_SIDE>::right, Checks>(pos, moveList, Us);
  }

  return moveList;
}

template<Color C, CastlingSide S> struct MakeCastling {
  static constexpr CastlingRight
    right = C == WHITE ? S == QUEEN_SIDE ? WHITE_OOO : WHITE_OO
    : S == QUEEN_SIDE ? BLACK_OOO : BLACK_OO;
};


// Reduce target to destination square only. It is harmless for castling
// moves because generate_castling() does not use target.

static inline Bitboard trim(Bitboard target, const char* san) {

  if (!san[3] || san[3] == '+')
    return target & make_square(File(san[1] - 'a'), Rank(san[2] - '1'));
  return target;
}

static inline Bitboard trimPawn(Bitboard target, const char* san, bool isCapture) {

  if (isCapture)
  {
    if (san[1] == 'x')
      return target & make_square(File(san[2] - 'a'), Rank(san[3] - '1'));
    else
      // Wrong notation, possibly a uci move like d4xf6, in this case retrun
      // empty target becuase strict search will not find it anyhow
      return 0;
  }
  else
    return target & file_bb(File(san[0] - 'a'));

  return target;
}

template<Color Us, GenType Type, bool Checks>
ExtMove* generate_castling_moves(const Position& pos, ExtMove* moveList) {

  if (Type != CAPTURES && Type != EVASIONS && pos.can_castle( CastlingRight( pos.castling_rights( Us ) ) ))
  {
    moveList = generate_castling<MakeCastling<Us, KING_SIDE>::right, Checks>(pos, moveList, Us);
    moveList = generate_castling<MakeCastling<Us, QUEEN_SIDE>::right, Checks>(pos, moveList, Us);
  }

  return moveList;
}

Move BoardHistory::san_to_move(Position const &pos, const std::string& s) const {
  const char* cur = &s[0];
  ExtMove moveList[MAX_MOVES];
  ExtMove* last;
  Color us = pos.side_to_move();

  bool isCapture = strchr(cur, 'x');
  Bitboard target = isCapture ? pos.pieces(~us) : ~pos.pieces();

  switch (cur[0]) {
  case 'N':
    last = generate_moves<KNIGHT, false>(pos, moveList, us, trim(target, cur));
    break;

  case 'B':
    last = generate_moves<BISHOP, false>(pos, moveList, us, trim(target, cur));
    break;

  case 'R':
    last = generate_moves<ROOK, false>(pos, moveList, us, trim(target, cur));
    break;

  case 'Q':
    last = generate_moves<QUEEN, false>(pos, moveList, us, trim(target, cur));
    break;

  case 'K':
    last = us == WHITE ? generate_king_moves<WHITE, NON_EVASIONS, false, false>(pos, moveList, trim(target, cur))
      : generate_king_moves<BLACK, NON_EVASIONS, false, false>(pos, moveList, trim(target, cur));
    break;

  case 'O':
  case '0':
  case 'o':
    last = us == WHITE ? generate_castling_moves<WHITE, NON_EVASIONS, false>(pos, moveList)
      : generate_castling_moves<BLACK, NON_EVASIONS, false>(pos, moveList);
    break;

  case '-':
    assert(!strcmp(cur, "--"));
    return MOVE_NULL;

  default:
    assert(cur[0] >= 'a' && cur[0] <= 'h');

    target = trimPawn(target, cur, isCapture);

    if (isCapture)
      last = us == WHITE ? generate_pawn_moves<WHITE, CAPTURES>(pos, moveList, target)
      : generate_pawn_moves<BLACK, CAPTURES>(pos, moveList, target);
    else
      last = us == WHITE ? generate_pawn_moves<WHITE, QUIETS>(pos, moveList, target)
      : generate_pawn_moves<BLACK, QUIETS>(pos, moveList, target);
    break;
  }

  for (ExtMove* m = moveList; m < last; ++m)
    if (move_is_san(pos, m->move, cur) && pos.legal(m->move))
      return m->move;

 return MOVE_NONE;
}

// count number of legal nodes up to a given depth from a given position.
uint64_t UCI::perft(BoardHistory& bh, Depth depth, bool Root) {

  StateInfo st;
  uint64_t cnt, nodes = 0;
  const bool leaf = (depth == 2 * ONE_PLY);

  for (const auto& m : MoveList<LEGAL>(bh.cur()))
  {
    if (Root && depth <= ONE_PLY)
      cnt = 1, nodes++;
    else
    {
      bh.cur().do_move(m, st);
      cnt = leaf ? MoveList<LEGAL>(bh.cur()).size() : UCI::perft(bh, depth - ONE_PLY, false);
      nodes += cnt;
      bh.cur().undo_move(m);
    }
  }
  return nodes;
}

