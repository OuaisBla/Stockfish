#include "pch.h"
#include "BoardHistory.h"
#include "Thread.h"


void BoardHistory::set(const std::string& fen) {

  positions.clear();
  states.clear();
  moves.clear();
  moveStats.clear();

  moves.push_back(Move::MOVE_NONE);
  moveStats.push_back(MoveStats());

  states.emplace_back(new StateInfo());

  position.set(fen, false, states.back().get(), Threads.main());

  positions.push_back(fen);
}

void BoardHistory::do_move(Move m) {
  do_move(m, MoveStats());
}

void BoardHistory::do_move(Move m, MoveStats stats)
{
  states.emplace_back(new StateInfo);

  moves.push_back(m);
  moveStats.push_back(stats);

  position.do_move(m, *states.back());

  positions.push_back(position.fen());
}

bool BoardHistory::undo_move() {
  if (moves.size() <= 1) return false;

  position.undo_move(moves.back());

  moves.pop_back();
  moveStats.pop_back();
  states.pop_back();
  positions.pop_back();

  return true;
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

    result += move_to_san(p, moves[i + 1]) + " ";
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
