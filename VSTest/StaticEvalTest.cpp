#include "pch.h"
#include "gtest/gtest.h"
#include "pgn.h"
#include "Bitboard.h"
#include "Position.h"
#include "UCI.h"
#include "evaluate.h"
#include "thread.h"
#include "search.h"
#include "EngineInit.h"


class StaticEvalTest : public ::testing::Test {
protected:

  static void SetUpTestCase() {

    EngineInit();

  }

  static void TearDownTestCase()
  {

    EngineReset();

  }

};

TEST_F(StaticEvalTest, TestStaticEval_CC7_Blitz_Bonanza_final_game_37_move107) {

  const char fen[] = "6r1/1p6/7b/3p1b1P/B2Ppkp1/P1P2p2/1P3P2/4K1RR w - - 4 101";

  StateInfo st;

  Position pos;
  pos.set(fen, false, &st, Threads.main());

  Value val = Eval::evaluate(pos);
  val = pos.side_to_move() == WHITE ? val : -val; // Trace scores are from white's point of view

  std::cout << Eval::trace(pos) << std::endl;

  EXPECT_TRUE( (double(val) / PawnValueEg) < 2);
}



TEST_F(StaticEvalTest, TestPos_CC9_TheGauntlet_Qualification_146_move157) {

  const char fen[] = "1QK5/6k1/1qB1R3/8/8/8/6p1/8 b - - 0 157";

  StateListPtr states(new std::deque<StateInfo>(1));

  Position pos;
  pos.set(fen, false, &states->back(), Threads.main());

  Search::LimitsType limits;
  limits.startTime = now(); // As early as possible!
  limits.depth = 1;

  Threads.start_thinking(pos, states, limits);

  Threads.main()->wait_for_search_finished();

  Move bestMove = Threads.main()->rootMoves[0].pv[0];

  EXPECT_EQ("b6b8", UCI::move(bestMove, false));
}

TEST_F(StaticEvalTest, TestPos_CC9_TheGauntlet_Final_142_move32) {

  const char fen[] = "r7/1q1r1p1k/3p1Qpp/pbpP4/1nB1PBP1/1P3N1P/5P1K/4R3 b - - 4 32";

  StateListPtr states(new std::deque<StateInfo>(1));

  Position pos;
  pos.set(fen, false, &states->back(), Threads.main());

  Search::LimitsType limits;
  limits.startTime = now(); // As early as possible!
  limits.depth = 1;

  Threads.start_thinking(pos, states, limits);

  Threads.main()->wait_for_search_finished();

  Move bestMove = Threads.main()->rootMoves[0].pv[0];

  std::string move = UCI::move(bestMove, false);

  std::cout << move << std::endl;

  Value val = Eval::evaluate(pos);
  val = pos.side_to_move() == WHITE ? val : -val; // Trace scores are from white's point of view

  std::cout << Eval::trace(pos) << std::endl;

  EXPECT_TRUE((double(val) / PawnValueEg) >= 0 );
}

