
#include "pch.h"

#include <gtest/gtest.h>
#include "pgn.h"
#include "Types.h"
#include "Bitboard.h"
#include "Position.h"
#include "UCI.h"
#include "Thread.h"
#include "Endgame.h"
#include "EngineInit.h"

class PerfTest: public ::testing::Test {
public:
  PerfTest() {

    EngineInit();

  }

};


template<bool Root>
uint64_t perft(Position& pos, Depth depth);



// FEN positions from https://chessprogramming.wikispaces.com/Perft+Results

TEST_F(PerfTest, InitPos) {
  BoardHistory bh_;
  bh_.set("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  auto n = perft<true>(bh_.cur(), Depth(4));
  EXPECT_EQ(n, 197'281);
}

TEST_F(PerfTest, Pos2) {
  BoardHistory bh_;
  bh_.set("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
  auto n = perft<true>(bh_.cur(), Depth(6));
  EXPECT_EQ(n, 8'031'647'685);
}

TEST_F(PerfTest, Pos3) {
  BoardHistory bh_;
  bh_.set("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");
  auto n = perft<true>(bh_.cur(), Depth(6));
  EXPECT_EQ(n, 11'030'083);
}

TEST_F(PerfTest, Pos4) {
  BoardHistory bh_;
  bh_.set("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
  auto n = perft<true>(bh_.cur(), Depth(5));
  EXPECT_EQ(n, 15'833'292);
}

TEST_F(PerfTest, Pos5) {
  BoardHistory bh_;
  bh_.set("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
  auto n = perft<true>(bh_.cur(), Depth(4));
  EXPECT_EQ(n, 2'103'487);
}

TEST_F(PerfTest, Pos6) {
  BoardHistory bh_;
  bh_.set("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
  auto n = perft<true>(bh_.cur(), Depth(4));
  EXPECT_EQ(n, 3'894'594);
}



// https://peterellisjones.com/posts/generating-legal-chess-moves-efficiently/

TEST_F(PerfTest, Pos7) {

    char const fen[] = "r6r/1b2k1bq/8/8/7B/8/8/R3K2R b QK - 3 2";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(1));
    EXPECT_EQ(n, 8);
}

TEST_F(PerfTest, Pos8) {

    char const fen[] = "8/8/8/2k5/2pP4/8/B7/4K3 b - d3 5 3";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(1));
    EXPECT_EQ(n, 8);
}

TEST_F(PerfTest, Pos9) {

    char const fen[] = "r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w QqKk - 2 2";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(1));
    EXPECT_EQ(n, 19);
}

TEST_F(PerfTest, Pos10) {

    char const fen[] = "r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b QqKk - 3 2";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(1));
    EXPECT_EQ(n, 5);
}

TEST_F(PerfTest, Pos11) {

    char const fen[] = "2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b QK - 3 2";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(1));
    EXPECT_EQ(n, 44);
}

TEST_F(PerfTest, Pos12) {

    char const fen[] = "rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w QK - 3 9";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(1));
    EXPECT_EQ(n, 39);
}


TEST_F(PerfTest, Pos13) {

    char const fen[] = "2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(1));
    EXPECT_EQ(n, 9);
}

TEST_F(PerfTest, Pos14) {

    char const fen[] = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(3));
    EXPECT_EQ(n, 62379);
}

TEST_F(PerfTest, Pos15) {

    char const fen[] = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(3));
    EXPECT_EQ(n, 89890);
}

TEST_F(PerfTest, Pos16) {

    char const fen[] = "3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(6));
    EXPECT_EQ(n, 1134888);
}

TEST_F(PerfTest, Pos17) {

    char const fen[] = "8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(6));
    EXPECT_EQ(n, 1015133);
}

TEST_F(PerfTest, Pos18) {

    char const fen[] = "8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(6));
    EXPECT_EQ(n, 1440467);
}

TEST_F(PerfTest, Pos19) {

    char const fen[] = "5k2/8/8/8/8/8/8/4K2R w K - 0 1";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(6));
    EXPECT_EQ(n, 661072);
}

TEST_F(PerfTest, Pos20) {

    char const fen[] = "3k4/8/8/8/8/8/8/R3K3 w Q - 0 1";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(6));
    EXPECT_EQ(n, 803711);
}

TEST_F(PerfTest, Pos21) {

    char const fen[] = "r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(4));
    EXPECT_EQ(n, 1274206);
}

TEST_F(PerfTest, Pos22) {

    char const fen[] = "r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(4));
    EXPECT_EQ(n, 1720476);
}

TEST_F(PerfTest, Pos23) {

    char const fen[] = "2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(6));
    EXPECT_EQ(n, 3821001);
}

TEST_F(PerfTest, Pos24) {

    char const fen[] = "8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(5));
    EXPECT_EQ(n, 1004658);
}

TEST_F(PerfTest, Pos25) {

    char const fen[] = "4k3/1P6/8/8/8/8/K7/8 w - - 0 1";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(6));
    EXPECT_EQ(n, 217342);
}

TEST_F(PerfTest, Pos26) {

    char const fen[] = "8/P1k5/K7/8/8/8/8/8 w - - 0 1";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(6));
    EXPECT_EQ(n, 92683);
}

TEST_F(PerfTest, Pos27) {

    char const fen[] = "K1k5/8/P7/8/8/8/8/8 w - - 0 1";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(6));
    EXPECT_EQ(n, 2217);
}

TEST_F(PerfTest, Pos28) {

    char const fen[] = "8/k1P5/8/1K6/8/8/8/8 w - - 0 1";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(7));
    EXPECT_EQ(n, 567584);
}

TEST_F(PerfTest, Pos29) {

    char const fen[] = "8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    auto n = perft<true>(position, Depth(4));
    EXPECT_EQ(n, 23527);
}
