#pragma once


#include "types.h"
#include "PGNHeader.h"



inline double ValueToCP(Value v)
{
  double val = 0;

  if (v == VALUE_NONE)
  {
    return std::numeric_limits<double>::infinity();
  }

  if (abs(v) < VALUE_MATE - MAX_PLY)
  {
    val = double(v) / PawnValueEg;
  }
  else
  {
    int mateCount = (v > 0 ? VALUE_MATE - v + 1 : -VALUE_MATE - v) / 2;

    val = (MAX_PLY - abs(mateCount)) * (v > 0 ? 1 : -1);
  }

  return val;
}


class PlyObserver
{
public:

  virtual void SetHeader(PGNHeader const &_H) = 0;
  
  void AddValueMoves(Color side, Value eval, Move move )
  {
    double baseCP = ValueToCP(eval);

    AddPly(side, baseCP, move );
  }

  virtual void AddPly(Color side, double eval, Move move ) = 0;


};

class NullEvalObserver : public PlyObserver
{
public:
  
  NullEvalObserver() = default;


  virtual void SetHeader(PGNHeader const &)
  {

  }

  virtual void AddPly(Color , double, Move )
  {
  }

  static NullEvalObserver NullEvalObserverInstance;

};
