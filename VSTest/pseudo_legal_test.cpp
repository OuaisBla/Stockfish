
#include "pch.h"

#include "pgn.h"
#include "Types.h"
#include "Bitboard.h"
#include "Position.h"
#include "UCI.h"
#include "Thread.h"
#include "Endgame.h"
#include "EngineInit.h"


Move test_legality(Position& pos, Depth depth);


class PseudoLegalTest: public ::testing::Test {
public:
  PseudoLegalTest() {

    EngineInit();

  }

  static void TestAllLegalMoves(Position& position, Depth depth)
  {
      Move m = test_legality(position, depth);

      bool alllegal = m == MOVE_NULL;

      if (!alllegal)
      {
          std::cout << UCI::move(m, false) << std::endl;

          std::cout << position.fen() << std::endl;
      }

      EXPECT_TRUE(alllegal);
  }

  static void TestAllIllegalMoves( Position& position, Depth depth)
  {
    auto moves = MoveList<LEGAL>(position);

    for (int i = 0; i <= 1 << 15; ++i)
    {
      Move m = static_cast<Move>(i);

      if ( !moves.contains(m) )
      {
        bool pseudo_legal = position.pseudo_legal(m);

        if (pseudo_legal && position.legal(m) )
        {
          std::cout << UCI::move(m, false) << std::endl;

          std::cout << position.fen() << std::endl;

          EXPECT_FALSE(true);

          return;
        }
      }

      else if (depth > 1)
      {
        StateInfo st;
        position.do_move(m, st);

        TestAllIllegalMoves(position, depth - 1);

        position.undo_move(m);
      }
    }
   
  }


};


Move test_legality(Position& pos, Depth depth) 
{
  for (const auto& m : MoveList<LEGAL>(pos))
  {
    bool pseudo_legal = pos.pseudo_legal(m);

    if (!pseudo_legal)
    {
      return m;
    }

    if ( depth > 1 )
    {
      StateInfo st;
      pos.do_move(m, st);

      bool legal = test_legality(pos, depth - 1);

      if (!legal)
      {
        return m;
      }

      pos.undo_move(m);
    }
  }

  return MOVE_NULL;
}


// FEN positions from https://www.chessprogramming.org/Perft_Results

TEST_F(PseudoLegalTest, InitPos) {
  
  char const fen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;

  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(4));

  TestAllIllegalMoves(position, Depth(4));
}

#ifdef NDEBUG
TEST_F(PseudoLegalTest, DISABLED_Pos2) {

  char const fen[] = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(6));

  TestAllIllegalMoves(position, Depth(6));
}

#endif
TEST_F(PseudoLegalTest, Pos3) {

  char const fen[] = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(6));

  TestAllIllegalMoves(position, Depth(6));
}

TEST_F(PseudoLegalTest, Pos4) {

  char const fen[] = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(5));

  TestAllIllegalMoves(position, Depth(5));
}

TEST_F(PseudoLegalTest, Pos5) {

  char const fen[] = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(4));

  TestAllIllegalMoves(position, Depth(4));
}

TEST_F(PseudoLegalTest, Pos6) {

  char const fen[] = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(4));

  TestAllIllegalMoves(position, Depth(4));
}

/*
TEST_F(PseudoLegalTest, InitPosDepth9) {

    char const fen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    StateInfo st;
    memset(&st, 0, sizeof st);

    Position position;
    position.set(fen, false, &st, Threads.main());

    TestAllLegalMoves(position, Depth(9));
}

*/



// https://peterellisjones.com/posts/generating-legal-chess-moves-efficiently/

TEST_F(PseudoLegalTest, Pos7) {

  char const fen[] = "r6r/1b2k1bq/8/8/7B/8/8/R3K2R b QK - 3 2";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(1));

  TestAllIllegalMoves(position, Depth(1));
}

TEST_F(PseudoLegalTest, Pos8) {

  char const fen[] = "8/8/8/2k5/2pP4/8/B7/4K3 b - d3 5 3";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(1));

  TestAllIllegalMoves(position, Depth(1));
}

TEST_F(PseudoLegalTest, Pos9) {

  char const fen[] = "r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w QqKk - 2 2";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(1));

  TestAllIllegalMoves(position, Depth(1));
}

TEST_F(PseudoLegalTest, Pos10) {

  char const fen[] = "r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b QqKk - 3 2";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(1));

  TestAllIllegalMoves(position, Depth(1));
}

TEST_F(PseudoLegalTest, Pos11) {

  char const fen[] = "2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b QK - 3 2";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(1));

  TestAllIllegalMoves(position, Depth(1));
}

TEST_F(PseudoLegalTest, Pos12) {

  char const fen[] = "rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w QK - 3 9";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(1));

  TestAllIllegalMoves(position, Depth(1));
}


TEST_F(PseudoLegalTest, Pos13) {

  char const fen[] = "2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(1));

  TestAllIllegalMoves(position, Depth(1));
}

TEST_F(PseudoLegalTest, Pos14) {

  char const fen[] = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(3));

  TestAllIllegalMoves(position, Depth(3));
}

TEST_F(PseudoLegalTest, Pos15) {

  char const fen[] = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(3));

  TestAllIllegalMoves(position, Depth(3));
}

TEST_F(PseudoLegalTest, Pos16) {

  char const fen[] = "3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(6));

  TestAllIllegalMoves(position, Depth(6));
}

TEST_F(PseudoLegalTest, Pos17) {

  char const fen[] = "8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(6));

  TestAllIllegalMoves(position, Depth(6));
}

TEST_F(PseudoLegalTest, Pos18) {

  char const fen[] = "8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(6));

  TestAllIllegalMoves(position, Depth(6));
}

TEST_F(PseudoLegalTest, Pos19) {

  char const fen[] = "5k2/8/8/8/8/8/8/4K2R w K - 0 1";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(6));

  TestAllIllegalMoves(position, Depth(6));
}

TEST_F(PseudoLegalTest, Pos20) {

  char const fen[] = "3k4/8/8/8/8/8/8/R3K3 w Q - 0 1";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(6));

  TestAllIllegalMoves(position, Depth(6));
}

TEST_F(PseudoLegalTest, Pos21) {

  char const fen[] = "r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(4));

  TestAllIllegalMoves(position, Depth(4));
}

TEST_F(PseudoLegalTest, Pos22) {

  char const fen[] = "r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(4));

  TestAllIllegalMoves(position, Depth(4));
}

TEST_F(PseudoLegalTest, Pos23) {

  char const fen[] = "2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(6));

  TestAllIllegalMoves(position, Depth(6));
}

TEST_F(PseudoLegalTest, Pos24) {

  char const fen[] = "8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(5));

  TestAllIllegalMoves(position, Depth(5));
}

TEST_F(PseudoLegalTest, Pos25) {

  char const fen[] = "4k3/1P6/8/8/8/8/K7/8 w - - 0 1";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(6));

  TestAllIllegalMoves(position, Depth(6));
}

TEST_F(PseudoLegalTest, Pos26) {

  char const fen[] = "8/P1k5/K7/8/8/8/8/8 w - - 0 1";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(6));

  TestAllIllegalMoves(position, Depth(6));
}

TEST_F(PseudoLegalTest, Pos27) {

  char const fen[] = "K1k5/8/P7/8/8/8/8/8 w - - 0 1";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(6));

  TestAllIllegalMoves(position, Depth(6));
}

TEST_F(PseudoLegalTest, Pos28) {

  char const fen[] = "8/k1P5/8/1K6/8/8/8/8 w - - 0 1";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(7));

  TestAllIllegalMoves(position, Depth(7));
}

TEST_F(PseudoLegalTest, Pos29) {

  char const fen[] = "8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1";

  StateInfo st;
  memset(&st, 0, sizeof st);

  Position position;
  position.set(fen, false, &st, Threads.main());

  TestAllLegalMoves(position, Depth(7));

  TestAllIllegalMoves(position, Depth(4));
}
