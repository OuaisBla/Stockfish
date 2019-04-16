#pragma once

#include "BoardHistory.h"
#include "MoveStats.h"


struct PGNGame {

  PGNGame() {}
  PGNGame(PGNGame const &) = delete;

  BoardHistory bh;
  int result;
};

class PGNParser {
 public:

  static const char* StartFEN;

  PGNParser(std::istream& is);

  std::unique_ptr<PGNGame> parse();

 private:
  std::istream& is_;
};


namespace UCI {

  uint64_t perft(BoardHistory& bh, Depth depth, bool Root = true);

}
