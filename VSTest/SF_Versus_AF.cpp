#include "pch.h"
#include <gtest/gtest.h>
#include "pgn.h"
#include "bitboard.h"
#include "position.h"
#include "search.h"
#include "thread.h"
#include "evaluate.h"
#include "tt.h"
#include "uci.h"
#include <fstream>      // std::filebuf
#include <csignal>
#include "CSVEval.h"

/*

void signalHandler(int signum) {
  std::cout << "Interrupt signal (" << signum << ") received.\n";

  // cleanup and close up stuff here  
  // terminate program  

  abort();
}


class SF_Versus_AF_Test : public ::testing::Test {
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

    Options["Threads"] = std::string("6");
    Options["Hash"] = std::string("4096");

  // register signal SIGABRT and signal handler to be able to catch assert with a breakpoint
    signal(SIGABRT, signalHandler);

    std::cout << engine_info(false) << std::endl;
  }

  static void TearDownTestCase() 
  {
    Threads.set(0);
  }

  static Value PlayGame(std::string const &pgn, EvalObserver &evalobserver = NullEvalObserver::NullEvalObserverInstance);

};

TEST_F(SF_Versus_AF_Test, TestGame_1_Parser) {

  std::filebuf fb;
  if (fb.open("SF-AF_1.pgn", std::ios::in))
  {
    std::istream is(&fb);
    
    PGNParser parser(is);

    auto game = parser.parse();

    EXPECT_EQ(game->result, 0 );
  }
  else
  {
    EXPECT_TRUE(false);
  }
}

TEST_F(SF_Versus_AF_Test, TestGame_1) {

  Value score = PlayGame("SF-AF_1.pgn");

  EXPECT_EQ(score, VALUE_DRAW );
}

TEST_F(SF_Versus_AF_Test, TestGame_2) {

  Value score = PlayGame("SF-AF_2.pgn");

  EXPECT_EQ(score, VALUE_DRAW); 
}

TEST_F(SF_Versus_AF_Test, TestPos_CC7_Blitz_Bonanza_final_game_37_move107) {

  const char fen[] = "6r1/1p6/7b/3p1b1P/B2Ppkp1/P1P2p2/1P3P2/4K1RR w - - 4 101";
  
  bool ponderMode = false;

  StateListPtr states(new std::deque<StateInfo>(1));

  Position pos;
  pos.set(fen, false, &states->back(), Threads.main());

  std::string trace = Eval::trace(pos);

  std::cout << trace << std::endl;

  Value staticEval = Eval::evaluate(pos);
  staticEval = pos.side_to_move() == WHITE ? staticEval : -staticEval; // Trace scores are from white's point of view


  {
    Pawns::Entry *pe = Pawns::probe(pos);

    std::cout << "Pawns attack." << std::endl;
    std::cout << Bitboards::pretty( pe->pawn_attacks_span(pos.side_to_move()) );

    std::cout << "Pawns attack." << std::endl;
    std::cout << Bitboards::pretty(pe->pawn_attacks_span(~pos.side_to_move()));

    std::cout << "Passed pawn." << std::endl;
    std::cout << Bitboards::pretty(pe->passed_pawns(pos.side_to_move()));

    std::cout << "Passed pawn." << std::endl;
    std::cout << Bitboards::pretty(pe->passed_pawns(~pos.side_to_move()));

    std::cout << "Passed pawn span." << std::endl;
    std::cout << Bitboards::pretty(pe->pawn_attacks_span(pos.side_to_move()));

    std::cout << "Passed pawn span." << std::endl;
    std::cout << Bitboards::pretty(pe->pawn_attacks_span(~pos.side_to_move()));
  }



  Search::LimitsType limits;
  limits.startTime = now(); // As early as possible!
  limits.depth = 24;

  Threads.start_thinking(pos, states, limits, ponderMode);

  Threads.main()->wait_for_search_finished();

  Move bestMove = Threads.main()->rootMoves[0].pv[0];

  std::cout << "Best move: " << UCI::move(bestMove, false) << std::endl;

  Value bestScore = Threads.main()->rootMoves[0].score;

  
  EXPECT_LT(bestScore, VALUE_DRAW );

  EXPECT_LT(staticEval, VALUE_DRAW);
}


Value SF_Versus_AF_Test::PlayGame(std::string const &pgn, EvalObserver &evalobserver)
{
  std::filebuf fb;
  fb.open(pgn.c_str(), std::ios::in);

  EXPECT_TRUE(fb.is_open());

  std::istream is(&fb);

  PGNParser parser(is);

  auto game = parser.parse();

  bool ponderMode = false;

  for (size_t i = 0; i < game->bh.moves.size() - 1; i++)
  {
    StateListPtr states(new std::deque<StateInfo>(1));

    Position pos;
    pos.set(game->bh.positions[i], false, &states->back(), Threads.main());

    Search::LimitsType limits;
    limits.startTime = now(); // As early as possible!

    // Get the depth according to move originally played.
    limits.depth = game->bh.moveStats[i].Depth;

    Threads.start_thinking(pos, states, limits, ponderMode);

    Threads.main()->wait_for_search_finished();

    Move bestMove = Threads.main()->rootMoves[0].pv[0];

    Value actualScore = game->bh.moveStats[i].value();

    Value bestScore = Threads.main()->rootMoves[0].score;

    std::cout << "Best score: " << UCI::value(bestScore) << std::endl;

    Move m = game->bh.moves[i + 1];

    if (m == bestMove)
    {
      std::cout << "Best move equal: " << UCI::move(m, false) << std::endl;
    }
    else
    {
      std::cout << "Best move recorded: " << UCI::move(m, false) << std::endl;
      std::cout << "Best move searched: " << UCI::move(bestMove, false) << std::endl;
    }

    evalobserver.AddMoves(pos.side_to_move(), actualScore, bestScore);
  }

  Value const finalScore = Threads.main()->rootMoves[0].score;

  return finalScore;
}
*/