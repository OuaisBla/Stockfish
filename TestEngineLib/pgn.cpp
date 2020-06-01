#include "stdafx.h"
#include "pgn.h"
#include "movegen.h"
#include <fstream>
#include <string>
#include <list>
#include <utility>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <map>
#include <sstream>


using namespace std;


const char* PGNParser::StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

namespace {

	const string PieceToSAN(" PNBRQK  PNBRQK");

}

template<bool Root>
uint64_t perft(Position& pos, Depth depth);


// trim from start (in place)
static inline void ltrim(string &s) {
  s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch) {
    return !isspace(ch) && ch != ',';
  }));
}

// trim from end (in place)
static inline void rtrim(string &s) {
  s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
    return !isspace(ch) && ch != ',';
  }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(string &s) {
  ltrim(s);
  rtrim(s);
}

bool is_result(const string& result) {
  return 
    result == "*" ||
    result == "1-0" || 
    result == "0-1" || 
    result == "1/2-1/2";
}

int parse_result(const string& result) {
  if (result == "1-0") {
    return 1;
  } else if (result == "0-1") {
    return -1;
  } else if (result == "1/2-1/2") {
    return 0;
  } else if (result == "*") {
    return 0;
  }

  throw runtime_error("Unknown result: " + result);
}

PGNHeader const & PGNGame::GetHeader() const
{
  return header;
}

void PGNGame::SetHeader(PGNHeader const &value)
{
  header = value;
}


BoardHistory const &PGNGame::GetBoardHistory() const
{
  return bh;
}


BoardHistory &PGNGame::GetBoardHistory()
{
  return bh;
}

EvalStacker PGNGame::EvalGraph() const
{
  EvalStacker values;

  values.SetHeader(header);

  Color c = WHITE;

  for (int i = 0; i < bh.moves.size(); ++i)
  {
    values.AddPly(c, bh.moveStats[i].value(), bh.moves[i] );

    c = ~c;
  }

  return values;
}

TournamentHeader const &PGNTournament::Header() const
{
  return header;
}



void PGNTournament::AddGame(std::shared_ptr<PGNGame> game)
{
  if (game->GetHeader().PlyCount > 0)
  {
    header = game->GetHeader();

    games.push_back(game);
  }
}

void PGNTournament::AddGames(std::vector<std::shared_ptr<PGNGame>> const &_g)
{
  games.insert(games.end(), _g.begin(), _g.end() );
}

std::vector<std::shared_ptr<PGNGame>> const & PGNTournament::GetGames() const
{
  return games;
}

std::vector<std::shared_ptr<PGNGame>> PGNTournament::GetNoCrashGames() const
{
  std::vector<std::shared_ptr<PGNGame>> sf_games;

  for (auto game : games)
  {
    if ( game->GetHeader().NoCrashEnding() )
    {
      sf_games.push_back(game);
    }
  }

  return sf_games;
}

// Return the list of games that match non empty PGNHeader values;
std::vector<std::shared_ptr<PGNGame>> PGNTournament::GetGames(std::string const & player) const
{
  std::vector<std::shared_ptr<PGNGame>> sf_games;

  for (auto game : games)
  {
    if (game->GetHeader().HasPlayed(player) &&
      game->GetHeader().NoCrashEnding())
    {
      sf_games.push_back(game);
    }
  }

  return sf_games;
}

std::vector<std::string> PGNTournament::GetPlayerNames() const
{
  std::vector<std::string> values;

  for (auto game : games)
  {
    auto whiteIt = std::find(values.begin(), values.end(), game->GetHeader().WhitePlayer);

    if ( whiteIt == values.end() )
    {
      values.push_back(game->GetHeader().WhitePlayer);
    }

    auto blackIt = std::find(values.begin(), values.end(), game->GetHeader().BlackPlayer);

    if (blackIt == values.end())
    {
      values.push_back(game->GetHeader().BlackPlayer);
    }
  }

  return values;
}

CrossTable PGNTournament::GetCrossTable() const
{
  std::vector<std::string> playernames = GetPlayerNames();

  CrossTable ct(playernames);

  for (size_t i = 0; i < playernames.size(); i++)
  {
    for (size_t j = 0; j < playernames.size(); j++)
    {
      if (i != j)
      {
        ct.AddScore(GetStatistics(playernames[i], playernames[j]));
      }
    }
  }

  return ct;
}

GameStats PGNTournament::GetStatistics(std::string const &p1, std::string const &p2) const
{
  int wins = 0, losses = 0, draws = 0;

  for (auto game : games)
  {
    if (p1 == game->GetHeader().WhitePlayer &&
      p2 == game->GetHeader().BlackPlayer)
    {
      draws += game->GetHeader().Result == 0;
      wins += game->GetHeader().Result == 1;
      losses += game->GetHeader().Result == -1;
    }
    else if (p2 == game->GetHeader().WhitePlayer &&
      p1 == game->GetHeader().BlackPlayer)
    {
      draws += game->GetHeader().Result == 0;
      wins += game->GetHeader().Result == -1;
      losses += game->GetHeader().Result == 1;
    }
  }

  return GameStats(p1, p2, wins, losses, draws);
}

std::vector<PlayerScore> PGNTournament::ELOScores(int baseElo, int K) const
{
  std::vector<PlayerScore> scores;

  if (games.empty())
  {
    return scores;
  }
  
  std::vector<std::string> playernames = GetPlayerNames();

  for (auto name : playernames)
  {
    scores.push_back(PlayerScore( name, double(baseElo) ));
  }

  for (auto game : games)
  {
    auto iit = std::find_if(scores.begin(), scores.end(), [&game](PlayerScore const &ps) { return ps.Name == game->GetHeader().WhitePlayer; });
    auto jit = std::find_if(scores.begin(), scores.end(), [&game](PlayerScore const &ps) { return ps.Name == game->GetHeader().BlackPlayer; });

    size_t i = iit - scores.begin();
    size_t j = jit - scores.begin();

    double R1 = std::pow(10.0, scores[i].Score / 400.0);

    double R2 = std::pow(10.0, scores[j].Score / 400.0);

    double E1 = R1 / (R1 + R2);

    double E2 = R2 / (R1 + R2);

    double S1 = double(game->GetHeader().Result + 1) / 2.0;

    double S2 = 1.0 - S1;

    scores[i].Score += K * (S1 - E1);

    scores[j].Score += K * (S2 - E2);
  }

  return scores;
}

template <typename Predicate>
std::string PGNTournament::GetCrossTable( Predicate pred ) const
{
  std::vector<std::string> playernames = GetPlayerNames();

  typedef std::multimap<std::string, GameStats> Maps;

  Maps stats;

  for (size_t i  = 0; i < playernames.size(); i++ )
  {
    for (size_t j = 0; j < playernames.size(); j++)
    {
      if (i != j)
      {
        GameStats stat = GetStatistics(playernames[i], playernames[j]);

        stats.insert(Maps::value_type(playernames[i], stat));
      }
    }
  }

  std::stringstream values;

  values << " ," ;

  for (auto const &p : playernames)
  {
    values << p << ",";
  }

  values << std::endl;

  for (auto const &p1 : playernames)
  {
    auto r = stats.equal_range(p1);

    values << p1 << ",";

    for (auto const &p2 : playernames)
    {
      auto stat = std::find_if(r.first, r.second, [p2](Maps::value_type g) { return p2 == g.second.Players.second; } );

      if(stat != r.second)
      {
        values << pred( stat->second ) << ",";
      }
      else
      {
        values << ",";
      }
    }

    values << std::endl;
  }

  return values.str();
}

std::string PGNTournament::GetWinCrossTable() const
{
  return GetCrossTable([](GameStats g) { return g.Wins; });
}

std::string PGNTournament::GetLossCrossTable() const 
{
  return GetCrossTable([](GameStats g) { return g.Losses; });
}

std::string PGNTournament::GetDrawCrossTable() const
{
  return GetCrossTable([](GameStats g) { return g.Draws; });
}


const string kEventToken = "[Event \"";
const string kRoundToken = "[Round \"";
const string kSiteToken = "[Site \"";

const string kDateToken = "[Date \"";
const string kTimeControlToken = "[TimeControl \"";
const string kTimeToken = "[Time \"";
const string kGameDurationToken = "[GameDuration \"";
const string kGameEndTimeToken = "[GameEndTime \"";
const string kGameStartTimeToken = "[GameStartTime \"";

const string kWhiteToken = "[White \""; 
const string kWhiteTypeToken = "[WhiteType \"";
const string kWhiteEloToken = "[WhiteElo \"";

const string kBlackToken = "[Black \"";
const string kBlackTypeToken = "[BlackType \"";
const string kBlackEloToken = "[BlackElo \"";

const string kECOToken = "[ECO \"";
const string kOpeningToken = "[Opening \"";
const string kVariationToken = "[Variation \"";
const string kFENToken = "[FEN \"";

const string kTerminationToken = "[Termination \"";
const string kPlyCountToken = "[PlyCount \"";

const string kResultToken = "[Result \"";



PGNHeader PGNParser::parseHeader(istream &is_)
{
  PGNHeader header;

  bool headerFound = false;

  for (;;) {

    string s;

    getline(is_, s);

    if (!is_.good()) {
      break;
    }

    if (s.empty() && !headerFound) {
      continue;
    }

    if (s.empty() && headerFound) {
      break;
    }

    if (s.substr(0, kEventToken.size()) == kEventToken) {
      headerFound = true;
      header.Event = s.substr(kEventToken.size(), s.size() - kEventToken.size() - 2);
    }

    else if (s.substr(0, kSiteToken.size()) == kSiteToken) {
      headerFound = true;
      header.Site = s.substr(kSiteToken.size(), s.size() - kSiteToken.size() - 2);
    }

    else if (s.substr(0, kDateToken.size()) == kDateToken) {
      headerFound = true;
      header.Date = s.substr(kDateToken.size(), s.size() - kDateToken.size() - 2);
    }

    /*else if (s.substr(0, kTimeToken.size()) == kTimeToken) {
      string result = s.substr(kTimeToken.size(), s.size() - kTimeToken.size() - 2);
    }*/

    else if (s.substr(0, kRoundToken.size()) == kRoundToken) {
      headerFound = true;
      header.Round = s.substr(kRoundToken.size(), s.size() - kRoundToken.size() - 2);
    }

    else if (s.substr(0, kWhiteToken.size()) == kWhiteToken) {
      headerFound = true;
      
      string name = s.substr(kWhiteToken.size(), s.size() - kWhiteToken.size() - 2);

      auto separator = name.find(' ');

      // remove version in the engine name
      header.WhitePlayer = name.substr(0, separator);

      while(separator < name.size() - 1 && !isdigit(name[separator + 1]))
      {
        auto start = separator + 1;

        separator = name.find(' ', start);

        header.WhitePlayer += " " + name.substr(start, separator);
      }
    }

    else if (s.substr(0, kBlackToken.size()) == kBlackToken) {
      headerFound = true;

      string name = s.substr(kBlackToken.size(), s.size() - kBlackToken.size() - 2);

      auto separator = name.find(' ');

      // remove version in the engine name
      header.BlackPlayer = name.substr(0, separator);

      while (separator < name.size() - 1 && !isdigit(name[separator + 1]))
      {
        auto start = separator + 1;

        separator = name.find(' ', start);

        header.BlackPlayer += " " + name.substr(start, separator);
      }
    }

    else if (s.substr(0, kWhiteEloToken.size()) == kWhiteEloToken) {
      headerFound = true;
      string result = s.substr(kWhiteEloToken.size(), s.size() - kWhiteEloToken.size() - 2);

      header.WhitePlayerELO = atoi(result.c_str());
    }

    else if (s.substr(0, kBlackEloToken.size()) == kBlackEloToken) {
      headerFound = true;
      string result = s.substr(kBlackEloToken.size(), s.size() - kBlackEloToken.size() - 2);
      
      header.BlackPlayerELO = atoi(result.c_str());
    }

    else if (s.substr(0, kECOToken.size()) == kECOToken) {
      headerFound = true;
      header.ECOCode = s.substr(kECOToken.size(), s.size() - kECOToken.size() - 2);
    }

    else if (s.substr(0, kOpeningToken.size()) == kOpeningToken) {
      headerFound = true;
      header.Opening = s.substr(kOpeningToken.size(), s.size() - kOpeningToken.size() - 2);
    }

    else if (s.substr(0, kFENToken.size()) == kFENToken) {
      headerFound = true;
      header.FEN = s.substr(kFENToken.size(), s.size() - kFENToken.size() - 2);
    }

    else if (s.substr(0, kVariationToken.size()) == kVariationToken) {
      headerFound = true;
      header.Variation = s.substr(kVariationToken.size(), s.size() - kVariationToken.size() - 2);
    }

    else if (s.substr(0, kTimeControlToken.size()) == kTimeControlToken) {
      headerFound = true;
      header.TimeControl = s.substr(kTimeControlToken.size(), s.size() - kTimeControlToken.size() - 2);
    }

    else if (s.substr(0, kTerminationToken.size()) == kTerminationToken) {
      headerFound = true;
      header.Termination = s.substr(kTerminationToken.size(), s.size() - kTerminationToken.size() - 2);
    }

    else if (s.substr(0, kPlyCountToken.size()) == kPlyCountToken) {
      headerFound = true;
      string result = s.substr(kPlyCountToken.size(), s.size() - kPlyCountToken.size() - 2);

      header.PlyCount = atoi(result.c_str());
    }

    else if (s.substr(0, kResultToken.size()) == kResultToken) {
      headerFound = true;

      string result = s.substr(kResultToken.size(), s.size() - kResultToken.size() - 2);

      header.Result = parse_result(result);
    }

  }

  return header;
}


unique_ptr<PGNTournament> PGNParser::parse(string const &file)
{

  filebuf fb;
  fb.open(file.c_str(), ios::in);

  istream is_(&fb);

  unique_ptr<PGNTournament> tournament(new PGNTournament);

  while (is_.good())
  {

    tournament->AddGame(parse(is_));

  }

  return tournament;
}


unique_ptr<PGNGame> PGNParser::parse(istream &is_)
{

  unique_ptr<PGNGame> game(new PGNGame);

  game->SetHeader(parseHeader(is_));

  if (game->GetHeader().FEN.empty())
  {
    game->GetBoardHistory().set(StartFEN);
  }
  else
  {
    game->GetBoardHistory().set(game->GetHeader().FEN);
  }

  // Read in the moves
  for (size_t i = 0;;) {
    
    string s;

    is_ >> s;

    if (!is_.good() || s.empty() || is_result(s)) {
      break;
    }

    // Parse comments with stats
    if (s.front() == '{' ) {

      list<string> stats;

      s = s.substr(1, s.length());
      
      if (!s.empty() && s.back() != '}')
      {
        stats.push_back(s);
      }

      while (s.empty() || s.back() != '}' )
      {
        is_ >> s;

        trim(s);

        // List of items.
        if (s.front() == '(')
        {
          string values = s.substr(1, s.length());

          while (s.empty() || s.back() != ')')
          {
            is_ >> s;

            values += " " + s;
          }

          s = values.substr(0, values.length() - 1);
        }

        if (!s.empty() && s.back() != '}' )
        {
          stats.push_back(s);
        }
      }

      s = s.substr(0, s.length()-1);

      if (!s.empty())
      {
        stats.push_back(s);
      }

      MoveStats movestats = MoveStatsParser::Parse(stats);

      game->GetBoardHistory().associateStats( i - 1, movestats);

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

    Move m = PGNParser::san_to_move(game->GetBoardHistory().cur(), s);

    if (m == MOVE_NONE && 
        game->GetHeader().Termination == "unterminated" ) 
    {
      return game;
    }
    else if (m == MOVE_NONE)
    {
      throw runtime_error("Unable to parse pgn move " + s);
    }

    game->GetBoardHistory().do_move(m);
  }

  return game;
}


string PGNParser::move_to_san(Position const& pos, Move m) {
	Square from = from_sq(m);
	Square to = to_sq(m);
	Piece pc = pos.moved_piece(m);

	string result = "";
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
bool PGNParser::move_is_san(Position const& pos, Move m, const char* ref) {

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
		others = b = (attacks_bb(type_of(pc), to, pos.pieces()) & pos.pieces(pos.side_to_move(), pt)) ^ from;

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

		else if (!(others & rank_bb(from))
      && (Strict && (ref[1] <= '8'))) // Check for wrong row disambiguation
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


Move PGNParser::san_to_move(Position const &pos, const string& s) {

  const char* cur = &s[0];

  for (const auto& m : MoveList<LEGAL>(pos))
    if (move_is_san(pos, m, cur) && pos.legal(m))
      return m;

 return MOVE_NONE;
}

