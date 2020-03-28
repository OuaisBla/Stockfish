#include "stdafx.h"
#include "GamePlayer.h"
#include "Thread.h"
#include <iostream>
#include "uci.h"


Value GamePlayer::PlayGame(std::shared_ptr<PGNGame> game, PlyObserver &evalobserver)
{

  evalobserver.SetHeader(game->GetHeader());

  for (size_t i = 0; i < game->GetBoardHistory().moves.size() - 1; i++)
  {
    StateListPtr states(new std::deque<StateInfo>(1));

    Position pos;
    pos.set(game->GetBoardHistory().positions[i], false, &states->back(), Threads.main());

    Search::LimitsType limits;
    limits.startTime = now(); // As early as possible!

    // Get the depth according to move originally played.
    limits.depth = std::min(game->GetBoardHistory().moveStats[i].Depth, MoveStats::DEFAULT_DEPTH);

    Threads.start_thinking(pos, states, limits);

    Threads.main()->wait_for_search_finished();

    Move bestMove = Threads.main()->rootMoves[0].pv[0];

    Value bestScore = Threads.main()->rootMoves[0].score;

    Move m = game->GetBoardHistory().moves[i + 1];

    if (m != bestMove)
    {
      std::cout << "Best move searched: " << UCI::move(bestMove, false) << std::endl;
    }

    evalobserver.AddPly(pos.side_to_move(), bestScore, bestMove);
  }

  Value const finalScore = Threads.main()->rootMoves[0].score;

  return finalScore;
}
