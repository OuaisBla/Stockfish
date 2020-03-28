#include "stdafx.h"
#include "BoardHistory.h"
#include "Thread.h"
#include "PGN.h"


void BoardHistory::set(const std::string& fen) {

  positions.clear();
  states.clear();
  moves.clear();
  moveStats.clear();

  states.emplace_back(new StateInfo());

  StateInfo &st = *states.back();
  memset(&st, 0, sizeof st);

  position.set(fen, false, &st, Threads.main());

  positions.push_back(fen);
}

void BoardHistory::do_move(Move m) {
  states.emplace_back(new StateInfo);

  StateInfo &st = *states.back();
  memset(&st, 0, sizeof st);

  moves.push_back(m);
  moveStats.push_back(MoveStats());

  position.do_move(m, st);

  positions.push_back(position.fen());
}

void BoardHistory::associateStats( size_t iMove, MoveStats stats)
{
  assert(iMove < moves.size());

  moveStats[iMove] = stats;
}

void BoardHistory::undo_move() {
  if (moves.empty()) return ;

  position.undo_move(moves.back());

  moves.pop_back();
  moveStats.pop_back();
  states.pop_back();
  positions.pop_back();
}

std::string BoardHistory::pgn() const {

  std::string result;
  for (int i = 0; i < static_cast<int>(moves.size()) - 1; ++i) {
    if (i % 2 == 0) {
      result += std::to_string(i / 2 + 1) + ". ";
    }

    StateInfo st;
    Position p;
    p.set(positions[i], false, &st, Threads.main() );

    result += PGNParser::move_to_san(p, moves[i + 1]) + " ";
  }
  int len = 0;
  int last_space = -1;
  for (int i = 0; i < static_cast<int>(result.size()); ++i) {
    if (result[i] == ' ') {
      last_space = i;
    }
    if (++len >= 76) {
      int remaining = i - last_space;
      result[last_space] = '\n';
      len = remaining;
    }
  }
  return result;
}
