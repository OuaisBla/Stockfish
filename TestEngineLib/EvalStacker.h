#pragma once

#include <vector>
#include <string>
#include "PlyObserver.h"
#include "PGNHeader.h"


class EvalStacker : public PlyObserver
{

  struct EvalMoves
  {
    double White = VALUE_NONE;
    double Black = VALUE_NONE;
  };

  std::vector<EvalMoves> moves;
  Color currentMove = WHITE;

  PGNHeader Header;

public:

  EvalStacker() = default;

  void SetHeader(PGNHeader const &_H) 
  {
    Header = _H;
  }

  void AddPly(Color c, double base, Move baseMove );

  std::string ToString() const;

  static EvalStacker Parse(std::string const &file);

  std::vector<double> GetWhiteMoves() const;

  std::vector<double> GetBlackMoves() const;

  std::vector<double> GetWhiteEvalCurve() const;

  std::vector<double> GetBlackEvalCurve() const;

};

