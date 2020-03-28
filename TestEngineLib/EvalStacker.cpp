#include "stdafx.h"
#include "EvalStacker.h"
#include <assert.h>
#include <fstream>
#include "csvparser.h"


void EvalStacker::AddPly(Color c, double base, Move move )
{
  assert(currentMove == c);

  if (currentMove == WHITE)
  {
    EvalMoves move;
    move.White = base;
  
    moves.push_back( move );

    currentMove = BLACK;
  }
  else
  {
    moves.back().Black = -base;

    currentMove = WHITE;
  }
}

std::string EvalStacker::ToString() const
{

  std::string str ( "Move," + Header.WhitePlayer + "," + Header.BlackPlayer + "\n" );

  for (int i = 0; i < moves.size(); i++)
  {
    EvalMoves const &move = moves[i];

    str += std::to_string(i + 1) + ",";
    str += std::to_string( move.White ) + ",";
    str += std::to_string( move.Black ) + "\n";
  }

  return str;
}

EvalStacker EvalStacker::Parse(std::string const &file)
{
  CsvParser *csvparser = CsvParser_new(file.c_str(), ",", 0);

  EvalStacker values;

  {
    CsvRow *row = CsvParser_getRow(csvparser);

    char const * const * rowFields = CsvParser_getFields(row);

    int const numCol = CsvParser_getNumFields(row);

    if (numCol != 3)
    {
      CsvParser_destroy_row(row);
      CsvParser_destroy(csvparser);

      throw std::runtime_error( "Invalid CSV file for evaluations: " + file );
    }

    PGNHeader header;
    header.WhitePlayer = rowFields[1];
    header.BlackPlayer = rowFields[2];

    values.SetHeader(header);

    CsvParser_destroy_row(row);
  }


  try
  {
    while (CsvRow *row = CsvParser_getRow(csvparser) )
    {
      char const * const * rowFields = CsvParser_getFields(row);

      int const numCol = CsvParser_getNumFields(row);

      if (numCol != 3)
      {
        CsvParser_destroy_row(row);
        CsvParser_destroy(csvparser);

        throw std::runtime_error("Invalid CSV file for evaluations: " + file);
      }

      double const baseEvalWhite = atof( rowFields[1] );

      values.AddPly(WHITE, baseEvalWhite, MOVE_NONE );

      double const baseEvalBlack = atof( rowFields[2] );

      values.AddPly(BLACK, baseEvalBlack, MOVE_NONE );


      CsvParser_destroy_row(row);
    }
  }
  catch (...)
  {
    CsvParser_destroy(csvparser);
    csvparser = NULL;
  }

  CsvParser_destroy(csvparser);

  return values;
}

std::vector<double> EvalStacker::GetWhiteMoves() const
{
  std::vector<double> values;

  for (size_t i = 0; i < moves.size(); i++)
  {
    values.push_back( double(i) + 1);
  }

  return values;
}

std::vector<double> EvalStacker::GetBlackMoves() const
{
  std::vector<double> values;

  for (size_t i = 0; i < moves.size(); i++)
  {
    if (moves[i].Black != VALUE_NONE)
    {
      values.push_back(double(i) + 1);
    }
  }

  return values;
}

std::vector<double> EvalStacker::GetWhiteEvalCurve() const
{
  std::vector<double> values;

  for (auto const & m : moves)
  {
    values.push_back(m.White);
  }

  return values;
}

std::vector<double> EvalStacker::GetBlackEvalCurve() const
{
  std::vector<double> values;

  for (auto const & m : moves)
  {
    if (m.Black != VALUE_NONE)
    {
      values.push_back(m.Black);
    }
  }

  return values;
}

