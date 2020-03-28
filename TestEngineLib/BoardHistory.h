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

  void set(const std::string& fen);
  void do_move(Move m);
  void associateStats(size_t iMove, MoveStats stats );
  void undo_move();

  std::string pgn() const;

};


