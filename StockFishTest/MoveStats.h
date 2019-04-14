#pragma once


#include <string>
#include <list>
#include "types.h"


class MoveStats
{
public:

  MoveStats();

#ifdef _DEBUG
  static constexpr int DEFAULT_DEPTH = 11;
#else
  static constexpr int DEFAULT_DEPTH = 30;
#endif
  
  std::string Eval;
  int Depth = DEFAULT_DEPTH;

  Value value() const;

};


class MoveStatsParser
{
public:


  static MoveStats Parse(std::list<std::string> const &str);

};

