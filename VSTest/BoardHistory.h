#pragma once

#include <string>
#include <vector>
#include "Position.h"
#include "MoveStats.h"


struct BoardHistory {


  Position position;

  std::vector<std::string> positions;
  std::vector<Move> moves;
  std::vector<MoveStats> moveStats;
  std::vector<std::unique_ptr<StateInfo>> states;

  Position& cur() {
    return position;
  }

  const Position& cur() const {
    return position;
  }

  template<bool Strict = true>
  bool move_is_san(Position const& pos, Move m, const char* ref) const;

  std::string move_to_san(Position const& pos, Move m) const;
  Move san_to_move(Position const& pos, const std::string& s) const;

  void set(const std::string& fen);
  void do_move(Move m);
  void do_move(Move m, MoveStats stats);
  bool undo_move();
  std::string pgn() const;
};


