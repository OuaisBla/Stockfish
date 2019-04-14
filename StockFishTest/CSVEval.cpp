
#include "pch.h"
#include "CSVEval.h"
#include <assert.h>
#include <fstream>



void CSVEval::AddMoves(Color c, Value base, Value analysis)
{
  assert(currentMove == c);

  if (currentMove == WHITE)
  {
    moves.push_back( EvalMoves { base, analysis });

    currentMove = BLACK;
  }
  else
  {
    moves.back().BaseBlack = base;
    moves.back().AnalysisBlack = analysis;

    currentMove = WHITE;
  }
}

std::string CSVEval::ToString() const
{
  std::string str;

  for (int i = 0; i < moves.size(); i++)
  {
    EvalMoves const &move = moves[i];

    str += std::to_string(i + 1) + ",\t";
    str += std::to_string(double( move.BaseWhite ) / PawnValueEg) + ",\t";
    str += std::to_string(double( move.AnalysisWhite ) / PawnValueEg) + ",\t";
    str += std::to_string(double( move.BaseBlack ) / PawnValueEg) + ",\t";
    str += std::to_string(double( move.AnalysisBlack ) / PawnValueEg) + "\n";
  }

  return str;
}

void CSVEval::ToFile(std::string const &file) const
{
  std::ofstream o(file);

  o << ToString();

  o.close();
}
