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



class PuzzleTest : public ::testing::Test {
protected:

  static void SetUpTestCase() {

    EngineInit();

  }

  static void TearDownTestCase()
  {

    EngineReset();

  }

  void TestPosition(char const *fen, char const *bm, Search::LimitsType limits);


};

TEST_F(PuzzleTest, Nolot_Suite_Problem_1) {

  Search::LimitsType limits;
  limits.startTime = now(); // As early as possible!
  limits.depth = 10;

  TestPosition("r3qb1k/1b4p1/p2pr2p/3n4/Pnp1N1N1/6RP/1B3PP1/1B1QR1K1 w - -", "Nxh6", limits);

}

TEST_F(PuzzleTest, Nolot_Suite_Problem_2) {

  Search::LimitsType limits;
  limits.startTime = now(); // As early as possible!
  limits.depth = 30;

  TestPosition("r4rk1/pp1n1p1p/1nqP2p1/2b1P1B1/4NQ2/1B3P2/PP2K2P/2R5 w - -", "Rxc5", limits);

}

TEST_F(PuzzleTest, Nolot_Suite_Problem_3) {

  Search::LimitsType limits;
  limits.startTime = now(); // As early as possible!
  limits.depth = 30;

  TestPosition("r2qk2r/ppp1b1pp/2n1p3/3pP1n1/3P2b1/2PB1NN1/PP4PP/R1BQK2R w KQkq -", "Nxg5", limits);

}

TEST_F(PuzzleTest, Nolot_Suite_Problem_4) {

  Search::LimitsType limits;
  limits.startTime = now(); // As early as possible!
  limits.depth = 30;

  TestPosition("r1b1kb1r/1p1n1ppp/p2ppn2/6BB/2qNP3/2N5/PPP2PPP/R2Q1RK1 w kq -", "Nxe6", limits);

}

TEST_F(PuzzleTest, Nolot_Suite_Problem_5) {

  Search::LimitsType limits;
  limits.startTime = now(); // As early as possible!
  limits.depth = 30;

  TestPosition("r2qrb1k/1p1b2p1/p2ppn1p/8/3NP3/1BN5/PPP3QP/1K3RR1 w - -", "e5", limits);

}

TEST_F(PuzzleTest, Nolot_Suite_Problem_6) {

  Search::LimitsType limits;
  limits.startTime = now(); // As early as possible!
  limits.depth = 30;

  TestPosition("rnbqk2r/1p3ppp/p7/1NpPp3/QPP1P1n1/P4N2/4KbPP/R1B2B1R b kq -", "axb5", limits);

}

TEST_F(PuzzleTest, Nolot_Suite_Problem_7) {

  Search::LimitsType limits;
  limits.startTime = now(); // As early as possible!
  limits.depth = 30;

  TestPosition("1r1bk2r/2R2ppp/p3p3/1b2P2q/4QP2/4N3/1B4PP/3R2K1 w k -", "Rxd8+", limits);

}

TEST_F(PuzzleTest, Nolot_Suite_Problem_8) {

  Search::LimitsType limits;
  limits.startTime = now(); // As early as possible!
  limits.depth = 40;

  TestPosition("r3rbk1/ppq2ppp/2b1pB2/8/6Q1/1P1B3P/P1P2PP1/R2R2K1 w - -", "Bxh7+", limits);

}

TEST_F(PuzzleTest, DISABLED_Nolot_Suite_Problem_9) {

  Search::LimitsType limits;
  limits.startTime = now(); // As early as possible!
  limits.depth = 40;

  Options["Hash"] = std::to_string(16324);
  Options["Threads"] = "1";

  // Ng5 isn't consider the only best move for this position. SF calculates bestmove g4h6 ponder g7h6

  TestPosition("r4r1k/4bppb/2n1p2p/p1n1P3/1p1p1BNP/3P1NP1/qP2QPB1/2RR2K1 w - -", "Nxh6" /*"Ng5"*/, limits);

}

TEST_F(PuzzleTest, Nolot_Suite_Problem_10) {

  Search::LimitsType limits;
  limits.startTime = now(); // As early as possible!
  limits.depth = 30;

  TestPosition("r1b2rk1/1p1nbppp/pq1p4/3B4/P2NP3/2N1p3/1PP3PP/R2Q1R1K w - -", "Rxf7", limits);

}

TEST_F(PuzzleTest, Nolot_Suite_Problem_11) {

  Search::LimitsType limits;
  limits.startTime = now(); // As early as possible!
  limits.depth = 33;

  TestPosition("r1b3k1/p2p1nP1/2pqr1Rp/1p2p2P/2B1PnQ1/1P6/P1PP4/1K4R1 w - -", "Rxh6", limits);

}


void PuzzleTest::TestPosition( char const *fen, char const *bm, Search::LimitsType limits)
{
#ifdef NDEBUG
  StateListPtr states(new std::deque<StateInfo>(1));

  Position pos;
  pos.set(fen, false, &states->back(), Threads.main());

  Threads.start_thinking(pos, states, limits);

  Threads.main()->wait_for_search_finished();

  Move bestMove = Threads.main()->rootMoves[0].pv[0];

  EXPECT_EQ(std::string(bm), PGNParser::move_to_san(pos, bestMove));

  std::cout << "Puzzle evaluation" << std::endl;
  std::cout << Eval::trace(pos) << std::endl;

  StateInfo st;
  pos.do_move(bestMove, st);

  std::cout << "Puzzle solution found" << std::endl;
  std::cout << Eval::trace(pos) << std::endl;
#endif
}
