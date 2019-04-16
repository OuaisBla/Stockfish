#include "pch.h"
#include "MoveStats.h"
#include <cassert>


MoveStats::MoveStats()
{
  Eval = "0";
}

Value MoveStats::value() const
{
  Value val = VALUE_INFINITE;

  char const * c = &Eval.c_str()[0];

  int sign = 1;

  // Consume sign
  if (*c == '-')
  {
    ++c;

    sign = -1;
  }

  // Comsume mate in X
  if(*c == 'M')
  {
    val = VALUE_MATE;

    ++c;
  }

  if (val != VALUE_MATE)
  {
    double v = atof(c);

    val = Value( static_cast<int>(v * sign * static_cast<double>(PawnValueEg) ) );
  }
  else
  {
    int v = atoi(c);

    assert(v < MAX_PLY);

    val = Value( VALUE_MATE - v * 2 * sign );
  }

  assert(-VALUE_INFINITE < val && val < VALUE_INFINITE);

  return val;
}

MoveStats MoveStatsParser::Parse(std::list<std::string> const &values)
{
  MoveStats stats;

  auto eval = std::find_if(values.begin(), values.end(), [](std::string const &str) {
    return str.substr(0, 3) == "ev="; } );

  if (eval != values.end())
  {
    std::string const &str = *eval;

    stats.Eval = str.substr(3, str.length());
  }

  auto depth = std::find_if(values.begin(), values.end(), [](std::string const &str) {
    return str.substr(0, 2) == "d="; });

  if (depth != values.end())
  {
    stats.Depth = atoi(&(*depth)[2]);

    stats.Depth = std::min( MoveStats::DEFAULT_DEPTH, stats.Depth );
  }

  return stats;
}



