#include "pch.h"
#include "gtest/gtest.h"
#include "pgn.h"
#include "Bitboard.h"
#include "Position.h"
#include "UCI.h"
#include "evaluate.h"
#include "Thread.h"


class PassedPawnTest : public ::testing::Test {
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


void evaluatePassedPawn(Position const &pos, Bitboard &passedPawns, Color c)
{
  Bitboard ourPawns = pos.pieces(c, PAWN);
  Bitboard theirPawns = pos.pieces(~c, PAWN);

  Direction const Up = (c == WHITE ? NORTH : SOUTH);

  Square s;
  const Square* pl = pos.squares<PAWN>(c);

  while ((s = *pl++) != SQ_NONE)
  {
    File f = file_of(s);

    Bitboard stoppers = theirPawns & passed_pawn_span(c, s);
    Bitboard lever = theirPawns & PawnAttacks[c][s];
    Bitboard leverPush = theirPawns & PawnAttacks[c][s + Up];

    Bitboard neighbours = ourPawns & adjacent_files_bb(f);
    Bitboard phalanx = neighbours & rank_bb(s);
    Bitboard support = neighbours & rank_bb(s - Up);

    // Passed pawns will be properly scored in evaluation because we need
    // full attack info to evaluate them. Include also not passed pawns
    // which could become passed after one or two pawn pushes when are
    // not attacked more times than defended.
    if (!(stoppers ^ lever ^ leverPush)
      && (support || !more_than_one(lever))
      && popcount(phalanx) >= popcount(leverPush))
    { 
      passedPawns |= s;
    }

    else if (stoppers == square_bb(s + Up)
      && relative_rank(c, s) >= RANK_5)
    {
      Bitboard b = c == WHITE ? 
        (shift<NORTH>(support) & ~theirPawns) : 
        (shift<SOUTH>(support) & ~theirPawns);

      while (b)
      {
        if (!more_than_one(theirPawns & PawnAttacks[c][pop_lsb(&b)]))
        {
          passedPawns |= s;
        }
      }
    }

  }
}

void evaluatePassedPawn(const std::string &fen, Bitboard backwardPawn[])
{
  Position pos;
  StateInfo si;
  pos.set(fen, false, &si, Threads.main());

  Color const Us = pos.side_to_move();
  Color const Them = ~pos.side_to_move();

  evaluatePassedPawn(pos, backwardPawn[Us], Us);
  evaluatePassedPawn(pos, backwardPawn[Them], Them);

  std::cout << "White" << std::endl;
  std::cout << Bitboards::pretty(backwardPawn[WHITE]) << std::endl;

  std::cout << "Black" << std::endl;
  std::cout << Bitboards::pretty(backwardPawn[BLACK]) << std::endl;
}


TEST_F(PassedPawnTest, TestPassedPawn_1) {

  const char fen[] = "3k4/8/2p5/3p4/3P4/8/8/3K4 w - - 0 1";

  Bitboard passedPawns[COLOR_NB] = { 0, 0 };

  evaluatePassedPawn(fen, passedPawns);

  EXPECT_TRUE(!passedPawns[BLACK]);
  EXPECT_TRUE(!passedPawns[WHITE]);
}

TEST_F(PassedPawnTest, TestPassedPawn_2) {

  const char fen[] = "6r1/1p6/7b/3p1b1P/B2Ppkp1/P1P2p2/1P3P2/4K1RR w - - 4 101";

  Bitboard passedPawns[COLOR_NB] = { 0, 0 };

  evaluatePassedPawn(fen, passedPawns);

  EXPECT_TRUE(passedPawns[BLACK] & Square::SQ_F3);
  EXPECT_TRUE(passedPawns[WHITE] & Square::SQ_H5);
}

TEST_F(PassedPawnTest, TestPassedPawn_3) {

  const char fen[] = "5k2/4npp1/1Pp5/P1R5/7p/7P/r2r2P1/2R2K2 w - - 0 1";

  Bitboard passedPawns[COLOR_NB] = { 0, 0 };

  evaluatePassedPawn(fen, passedPawns);

  EXPECT_TRUE(passedPawns[WHITE] & Square::SQ_B6);
  EXPECT_TRUE(passedPawns[WHITE] & Square::SQ_A5);
  EXPECT_TRUE(passedPawns[BLACK] & Square::SQ_C6);

  EXPECT_FALSE(passedPawns[BLACK] & ~Square::SQ_C6);
  EXPECT_FALSE(passedPawns[WHITE] & ~Square::SQ_B6 & ~Square::SQ_A5);
}

TEST_F(PassedPawnTest, TestPassedPawn_4) {

  const char fen[] = "4k3/8/8/1P2P2p/2Pp1p1P/5PP1/8/4K3 w - - 0 1";

  Bitboard passedPawns[COLOR_NB] = { 0, 0 };

  evaluatePassedPawn(fen, passedPawns);

  EXPECT_TRUE(passedPawns[WHITE] & Square::SQ_B5);
  EXPECT_TRUE(passedPawns[WHITE] & Square::SQ_C4);
  EXPECT_TRUE(passedPawns[WHITE] & Square::SQ_E5);

  EXPECT_TRUE(passedPawns[BLACK] & Square::SQ_D4);

  EXPECT_FALSE(passedPawns[BLACK] & ~Square::SQ_D4);
  EXPECT_FALSE(passedPawns[WHITE] & ~Square::SQ_B5 & ~Square::SQ_C4 & ~Square::SQ_E5);
}

