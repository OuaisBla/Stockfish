#include "pch.h"
#include <gtest/gtest.h>
#include "thread.h"
#include "uci.h"
#include "evaluate.h"
#include <csignal>
#include "EvalStacker.h"
#include "EngineInit.h"
#include "GamePlayer.h"


static void signalHandler(int signum) {
  std::cout << "Interrupt signal (" << signum << ") received.\n";

  // cleanup and close up stuff here  
  // terminate program  

  abort();
}


class SF_Versus_AF_Test : public ::testing::Test {
protected:
  static void SetUpTestCase() {

    EngineInit();

#ifdef  NDEBUG
    Options["Hash"] = std::string("16384");
#else
    Options["Hash"] = std::string("2048");
#endif //  NDEBUG

    Options["Threads"] = "1";

  // register signal SIGABRT and signal handler to be able to catch assert with a breakpoint
    signal(SIGABRT, signalHandler);

    std::cout << engine_info(false) << std::endl;

    std::cout << Options << std::endl;
  }

  static void TearDownTestCase() 
  {
    
    EngineReset();

  }

};

/*
TEST_F(SF_Versus_AF_Test, TestGame_1) {

  EvalStacker eval;

  auto tournament = PGNParser::parse("SF-AF_1.pgn");

  auto game = tournament->GetGames().front();

  Value score = PlayGame(game, eval);

  eval.ToFile( "SF-AF_1_eval.csv" );

  EXPECT_EQ(score, VALUE_DRAW );
}

TEST_F(SF_Versus_AF_Test, TestGame_2) {

  EvalStacker eval;

  auto tournament = PGNParser::parse("SF-AF_2.pgn");

  auto game = tournament->GetGames().front();

  Value score = PlayGame(game, eval);

  eval.ToFile("SF-AF_2_eval.csv");

  EXPECT_EQ(score, VALUE_DRAW); 
}


TEST_F(SF_Versus_AF_Test, TestPos_CC9_TheGauntlet_Final_142_move32) {

  EvalStacker eval;

  auto tournament = PGNParser::parse("Leelenstein_vs_Stockfish_2019.07.10.pgn");

  auto game = tournament->GetGames().front();

  Value score = PlayGame(game, eval);

  eval.ToFile("Leelenstein_vs_Stockfish_2019.07.10.csv");

  EXPECT_EQ(score, VALUE_DRAW);
}
*/

TEST_F(SF_Versus_AF_Test, TestPos_CC7_Blitz_Bonanza_final_game_37_move107) {

  const char fen[] = "6r1/1p6/7b/3p1b1P/B2Ppkp1/P1P2p2/1P3P2/4K1RR w - - 4 101";
  
  StateListPtr states(new std::deque<StateInfo>(1));

  Position pos;
  pos.set(fen, false, &states->back(), Threads.main());

  Search::LimitsType limits;
  limits.startTime = now(); // As early as possible!
  limits.depth = MoveStats::DEFAULT_DEPTH;

  Threads.start_thinking(pos, states, limits);

  Threads.main()->wait_for_search_finished();

  Move bestMove = Threads.main()->rootMoves[0].pv[0];

  std::cout << "Best move: " << UCI::move(bestMove, false) << std::endl;

  Value bestScore = Threads.main()->rootMoves[0].score;

  
  EXPECT_LT(bestScore, VALUE_DRAW );
}
