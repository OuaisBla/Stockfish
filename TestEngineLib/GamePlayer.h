#pragma once

#include "pgn.h"


class GamePlayer
{
public:

  GamePlayer() = default;


  static Value PlayGame(std::shared_ptr<PGNGame> game, PlyObserver &evalobserver = NullEvalObserver::NullEvalObserverInstance);

};

