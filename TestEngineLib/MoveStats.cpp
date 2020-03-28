#include "stdafx.h"
#include "MoveStats.h"
#include <cassert>
#include <algorithm>


MoveStats::MoveStats()
{
  Eval = "0";
}

double MoveStats::value() const
{
  double val = 0.0;

  char const * c = &Eval.c_str()[0];

  int sign = 1;

  // Consume sign
  if (*c == '+')
  {
    ++c;
  }

  // Consume sign
  if (*c == '-')
  {
    ++c;

    sign = -1;
  }

  // Consume mate in X
  if(*c == 'M')
  {
    val = VALUE_MATE;

    ++c;
  }

  //if (abs(v) < VALUE_MATE - MAX_PLY)
  //  ss << "cp " << v * 100 / PawnValueEg;
  //else
  //  ss << "mate " << (v > 0 ? VALUE_MATE - v + 1 : -VALUE_MATE - v) / 2;


  if (val != VALUE_MATE)
  {
    val = atof(c);
  }
  else
  {
    int v = atoi(c);

    // Some engine shows high eval count.
    if (v >= 40)
    {
      v = 40;
    }

    val = double(PawnValueEg) + 10 * pow( 1.1, double(40 - v) );
  }

  return val * sign;
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

  if (eval == values.end())
  {
    // Earlier version for storing eval and depth.
    auto eval_on_depth = std::find_if(values.begin(), values.end(), [](std::string const &str) {
      return str.find('/') != -1; });

    if (eval_on_depth != values.end())
    {
      size_t const delimiter = (*eval_on_depth).find('/');

      stats.Eval = (*eval_on_depth).substr(0, delimiter);

      stats.Depth = atoi(&(*eval_on_depth)[delimiter + 1]);
    }
  }

  return stats;
}



