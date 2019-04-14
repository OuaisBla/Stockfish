#pragma once

#include <vector>
#include <string>
#include "EvalObserver.h"

class CSVEval : public EvalObserver
{

  struct EvalMoves
  {
    Value BaseWhite = VALUE_NONE;
    Value AnalysisWhite = VALUE_NONE;
    Value BaseBlack = VALUE_NONE;
    Value AnalysisBlack = VALUE_NONE;
  };

  std::vector<EvalMoves> moves;
  Color currentMove = WHITE;

public:

  CSVEval() = default;

  void AddMoves(Color c, Value base, Value analysis);

  std::string ToString() const;

  void ToFile(std::string const &file) const;

};

