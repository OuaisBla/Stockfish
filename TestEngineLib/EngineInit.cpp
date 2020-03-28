#include "stdafx.h"
#include "EngineInit.h"
#include "bitboard.h"
#include "position.h"
#include "search.h"
#include "thread.h"
#include "tt.h"
#include "uci.h"
#include "endgame.h"
#include "syzygy/tbprobe.h"


void EngineInit()
{

  UCI::init(Options);
  PSQT::init();
  Bitboards::init();
  Position::init();
  Bitbases::init();
  Endgames::init();
  Threads.set(Options["Threads"]);
  Search::clear(); // After threads are up

}


void EngineReset()
{
  Threads.stop = true;

  Search::clear();

  Threads.set(0);
}

