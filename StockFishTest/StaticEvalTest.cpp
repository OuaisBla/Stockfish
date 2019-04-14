#include "pch.h"
#include "gtest/gtest.h"
#include "pgn.h"
#include "Bitboard.h"
#include "Position.h"
#include "UCI.h"
#include "evaluate.h"
#include "thread.h"
#include "search.h"


class StaticEvalTest : public ::testing::Test {
protected:

  static void SetUpTestCase() {
    UCI::init(Options);
    PSQT::init();
    Bitboards::init();
    Position::init();
    Bitbases::init();
    Search::init();
    Threads.set(Options["Threads"]);
    Search::clear(); // After threads are up
  }

};

TEST_F(StaticEvalTest, TestStaticEval_CC7_Blitz_Bonanza_final_game_37_move107) {

  const char fen[] = "6r1/1p6/7b/3p1b1P/B2Ppkp1/P1P2p2/1P3P2/4K1RR w - - 4 101";

  StateListPtr states(new std::deque<StateInfo>(1));

  Position pos;
  pos.set(fen, false, &states->back(), Threads.main());

  Value val = Eval::evaluate(pos);
  val = pos.side_to_move() == WHITE ? val : -val; // Trace scores are from white's point of view

  std::cout << Eval::trace(pos) << std::endl;

  EXPECT_TRUE(val < VALUE_DRAW);
}
