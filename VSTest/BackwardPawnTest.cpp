#include "pch.h"
#include "gtest/gtest.h"
#include "pgn.h"
#include "Bitboard.h"
#include "Position.h"
#include "UCI.h"
#include "evaluate.h"
#include "Thread.h"


class BackwardPawnTest : public ::testing::Test {
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

void evaluateBackwardPawn(Position const &pos, Bitboard &backwardPawn, Color c)
{
  Bitboard ourPawns = pos.pieces(c, PAWN);
  Bitboard theirPawns = pos.pieces(~c, PAWN);

  Direction const Up = (c == WHITE ? NORTH : SOUTH);

  Square s;
  const Square* pl = pos.squares<PAWN>(c);

  while ((s = *pl++) != SQ_NONE)
  {
    Bitboard stoppers = theirPawns & passed_pawn_span(c, s);
    Bitboard leverPush = theirPawns & PawnAttacks[c][s + Up];

    // A pawn is backward when it is behind all pawns of the same color
    // on the adjacent files and cannot be safely advanced.
    Bitboard b1 = ourPawns & pawn_attack_span(~c, s + Up);
    Bitboard b2 = leverPush | (s + Up);
    Bitboard b3 = stoppers & b2;

    bool backward = !b1 && b3;

    if (backward)
    {
      backwardPawn |= s;
    }
  }
}

void evaluateBackwardPawn(const std::string &fen, Bitboard backwardPawn[])
{
  Position pos;
  StateInfo si;
  pos.set(fen, false, &si, Threads.main());

  Color const Us = pos.side_to_move();
  Color const Them = ~pos.side_to_move();

  evaluateBackwardPawn(pos, backwardPawn[Us], Us);
  evaluateBackwardPawn(pos, backwardPawn[Them], Them);

  std::cout << "White" << std::endl;
  std::cout << Bitboards::pretty(backwardPawn[WHITE]) << std::endl;

  std::cout << "Black" << std::endl;
  std::cout << Bitboards::pretty(backwardPawn[BLACK]) << std::endl;
}

TEST_F(BackwardPawnTest, TestBackwardPawn_1) {

  const char fen[] = "3k4/8/2p5/3p4/3P4/8/8/3K4 w - - 0 1";

  Bitboard backwardPawn[COLOR_NB] = { 0, 0 };

  evaluateBackwardPawn(fen, backwardPawn);

  EXPECT_TRUE(backwardPawn[BLACK] & Square::SQ_C6);
  EXPECT_TRUE(backwardPawn[WHITE] & Square::SQ_D4);
  EXPECT_FALSE(backwardPawn[WHITE] & Square::SQ_D5);
}

TEST_F(BackwardPawnTest, TestBackwardPawn_2) {

  const char fen[] = "6r1/1p6/7b/3p1b1P/B2Ppkp1/P1P2p2/1P3P2/4K1RR w - - 4 101";

  Bitboard backwardPawn[COLOR_NB] = { 0, 0 };

  evaluateBackwardPawn(fen, backwardPawn);

  EXPECT_TRUE(backwardPawn[WHITE] & Square::SQ_F2);
}

TEST_F(BackwardPawnTest, TestBackwardPawn_3) {

  const char fen[] = "2rq1rk1/1p1b1p1p/p2p1pp1/2n5/2P1P3/1PN3P1/PB1Q2BP/R5K1 w - - 0 1";

  Bitboard backwardPawn[COLOR_NB] = { 0, 0 };

  evaluateBackwardPawn(fen, backwardPawn);

  EXPECT_TRUE(backwardPawn[BLACK] & Square::SQ_D6);
  EXPECT_TRUE(backwardPawn[WHITE] & Square::SQ_E4);
}

TEST_F(BackwardPawnTest, TestBackwardPawn_4) {

  const char fen[] = "2rqbrk1/pp4n1/2n1p3/3pN1p1/3P1pP1/PP1B2Q1/1BP4P/R5RK w - - 0 1";

  Bitboard backwardPawn[COLOR_NB] = { 0, 0 };

  evaluateBackwardPawn(fen, backwardPawn);

  EXPECT_TRUE(backwardPawn[BLACK] & Square::SQ_E6);
  EXPECT_TRUE(backwardPawn[BLACK] & Square::SQ_G5);
  EXPECT_TRUE(!backwardPawn[WHITE]);
}

TEST_F(BackwardPawnTest, TestBackwardPawn_5) {

  const char fen[] = "2rq2k1/1p1nb1pp/p1p1prbn/P2p4/1P1P1B2/2P2N2/4BPPP/RN1QR1K1 w - - 0 1";

  Bitboard backwardPawn[COLOR_NB] = { 0, 0 };

  evaluateBackwardPawn(fen, backwardPawn);

  EXPECT_TRUE(backwardPawn[BLACK] & Square::SQ_E6);
  EXPECT_TRUE(backwardPawn[BLACK] & Square::SQ_B7);
  EXPECT_TRUE(backwardPawn[WHITE] & Square::SQ_C3);
}

TEST_F(BackwardPawnTest, TestBackwardPawn_6) {

  const char fen[] = "r1bqkb1r/pp3ppp/2np1n2/1N2p3/4P3/2N5/PPP2PPP/R1BQKB1R w - - 0 1";

  Bitboard backwardPawn[COLOR_NB] = { 0, 0 };

  evaluateBackwardPawn(fen, backwardPawn);

  EXPECT_TRUE(backwardPawn[BLACK] & Square::SQ_D6);
  EXPECT_TRUE(!backwardPawn[WHITE]);
}
