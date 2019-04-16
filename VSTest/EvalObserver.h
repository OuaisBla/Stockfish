#pragma once


#include "types.h"


class EvalObserver
{
public:

  
  virtual void AddMoves(Color c, Value base, Value analysis) = 0;


};

class NullEvalObserver : public EvalObserver
{
public:
  
  NullEvalObserver() = default;

  virtual void AddMoves(Color c, Value base, Value analysis)
  {
  }

  static NullEvalObserver NullEvalObserverInstance;

};
