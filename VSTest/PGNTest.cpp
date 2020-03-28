#include "pch.h"
#include <gtest/gtest.h>
#include "pgn.h"
#include "EvalStacker.h"
#include "EngineInit.h"
#include <csignal>
#include <intrin.h>
#include "TestUtil.h"



static void signalHandler(int signum) {

  std::cout << "Interrupt signal (" << signum << ") received.\n";

  // cleanup and close up stuff here  
  // terminate program  

  abort();
}


class PGNTest : public ::testing::Test {
protected:
  static void SetUpTestCase() {

    EngineInit();

    // register signal SIGABRT and signal handler to be able to catch assert with a breakpoint
    signal(SIGABRT, signalHandler);

  }

  static void TearDownTestCase() 
  {
    
    EngineReset();

  }

  static std::unique_ptr<PGNTournament> TestPGN(std::string const & file, int gamecount, TournamentHeader const &  = TournamentHeader() );

};

TEST_F(PGNTest, TestPGN_ccc_1_stage_1_games_1) {

  auto tournament = PGNParser::parse("ccc_1_stage_1_games_1.pgn");

  auto game = tournament->GetGames().front();

  EXPECT_EQ(game->GetBoardHistory().moves.size(), game->GetHeader().PlyCount);
  EXPECT_EQ(game->GetHeader().PlayerSide(Stockfish), BLACK);
  EXPECT_EQ(game->GetHeader().Result, -1);

}

TEST_F(PGNTest, TestPGN_SF_AF_1) {

  auto tournament = PGNParser::parse("SF-AF_1.pgn");

  auto game = tournament->GetGames().front();

  EXPECT_EQ(game->GetBoardHistory().moves.size(), game->GetHeader().PlyCount);
  EXPECT_EQ(game->GetHeader().PlayerSide(Stockfish), BLACK);
  EXPECT_EQ(game->GetHeader().Result, 0 );

}

TEST_F(PGNTest, TestPGN_SF_AF_2) {

  auto tournament = PGNParser::parse("SF-AF_2.pgn");

  auto game = tournament->GetGames().front();

  EXPECT_EQ(game->GetBoardHistory().moves.size(), game->GetHeader().PlyCount);
  EXPECT_EQ(game->GetHeader().PlayerSide(Stockfish), WHITE);
  EXPECT_EQ(game->GetHeader().Result, 0);
}

///Games of stage one:
///
/// Computer chess fans can now download the entire 569 - game PGN from stage one, 
/// which includes the 552 official games counted for the standings and 17 more that were 
/// affected by technical issues. Update: 550 official games and 19 unterminated
TEST_F(PGNTest, TestPGN_CCC1_stage_1) {

  auto tournament = TestPGN("ccc_1_stage_1_games.pgn", 569, TournamentHeader{ "CCCC 1: Rapid Rumble (15|5) Stage 1" } );

  EXPECT_EQ(tournament->Header().Event, "CCCC 1: Rapid Rumble (15|5) Stage 1");

  EXPECT_EQ(tournament->GetNoCrashGames().size(), 550);

  EXPECT_EQ(tournament->GetPlayerNames().size(), 24);

  GameStats stats = tournament->GetStatistics("Stockfish", "Houdini");

  EXPECT_EQ(stats.Wins, 1);
  EXPECT_EQ(stats.Losses, 0);
  EXPECT_EQ(stats.Draws, 1);

  TestUtil::ToFile("CCC1_stage_1_Wins.txt", tournament->GetWinCrossTable());
  TestUtil::ToFile("CCC1_stage_1_Losses.txt", tournament->GetLossCrossTable());
  TestUtil::ToFile("CCC1_stage_1_Draws.txt", tournament->GetDrawCrossTable());

  CrossTable ct = tournament->GetCrossTable();

  GameStats sfStats = ct.Statistics("Stockfish");

  std::cout << "Stockfish stats: " << sfStats.GetScoreString() << std::endl;
  std::cout << "Stockfish N: " << sfStats.N() << std::endl;
  std::cout << "Stockfish score: " << sfStats.Score() << std::endl;
  std::cout << "Stockfish score difference: " << sfStats.ScoreDifference() << std::endl;
  std::cout << "Stockfish win ratio: " << sfStats.WinRatio() << std::endl;
  std::cout << "Stockfish draw ratio: " << sfStats.DrawRatio() << std::endl;
  std::cout << "Stockfish ELO difference: " << sfStats.ELO_Difference() << std::endl;
  std::cout << "Stockfish LOS: " << sfStats.LikelyhoodOfSuperiority() * 100 << "%" << std::endl;

  // +32 -0 =15
  EXPECT_DOUBLE_EQ(ct.Score("Stockfish").Score, 39.5 );
}

TEST_F(PGNTest, PGN_With_Abandoned) {

  auto tournament = TestPGN("testing-trillionnode-testing.pgn", 60);

  EXPECT_EQ(tournament->GetGames().size(), tournament->GetNoCrashGames().size());

}

TEST_F(PGNTest, SF_Fire_WithFEN) {

  TestPGN("SF_Fire_WithFEN.pgn", 1 );

}

TEST_F(PGNTest, SF_K_Unterminated) {

  auto tournament = PGNParser::parse("SF_K_Unterminated.pgn");

  EXPECT_EQ(tournament->GetGames().size(), 1);

  auto game = tournament->GetGames().front();

  EXPECT_EQ(game->GetHeader().Event, "CCCC 1: Rapid Rumble (15|5) Stage 3");
  EXPECT_EQ(game->GetHeader().Site, "Chess.com");

  EXPECT_EQ(game->GetHeader().Termination, "unterminated");

  EXPECT_EQ(game->GetHeader().PlyCount, 2);
  EXPECT_LE(game->GetBoardHistory().moves.size(), game->GetHeader().PlyCount);

}

// CCC2 STAGE 3
// #	ENGINE	RATING	POINTS	PLAYED(%)
// 1	Stockfish 18111122	3449.7	177.5	300	59.33
// 2	Komodo 12.2.2	3390.2	145	300	48.17
// 3	Lc0 0.19	3381.4	139.5	300	46.5
// 4	Houdini 6.03	3378.7	138	300	46

TEST_F(PGNTest, TestPGN_CCC2_stage_3) {

  auto tournament = TestPGN("ccc_2_stage_3_games.pgn", 604);

  CrossTable ct = tournament->GetCrossTable();

  std::vector<PlayerScore> ps = tournament->ELOScores(3400, 10);

  for (auto const &s : ps)
  {
    GameStats stats = ct.Statistics(s.Name);

    std::cout << s.Name << " stats: " << stats.GetScoreString() << std::endl;
    std::cout << s.Name << " N: " << stats.N() << std::endl;
    std::cout << s.Name << " score: " << stats.Score() << std::endl;
    std::cout << s.Name << " score difference: " << stats.ScoreDifference() << std::endl;
    std::cout << s.Name << " win ratio: " << stats.WinRatio() << std::endl;
    std::cout << s.Name << " draw ratio: " << stats.DrawRatio() << std::endl;
    std::cout << s.Name << " ELO difference: " << stats.ELO_Difference() << std::endl;
    std::cout << s.Name << " LOS: " << stats.LikelyhoodOfSuperiority() * 100 << "%" << std::endl;

    std::cout << s.Name << " ELO: " << s.Score << std::endl;
  }

}

TEST_F(PGNTest, TestPGN_ccc11_bullet_test) {

  TestPGN("ccc11-bullet-test.pgn", 541);

}


std::unique_ptr<PGNTournament> PGNTest::TestPGN( std::string const & file, int gamecount, TournamentHeader const &header )
{
  auto tournament = PGNParser::parse(file);

  EXPECT_EQ(tournament->GetGames().size(), gamecount);

  for (auto game : tournament->GetGames())
  {
    if (!header.Event.empty())
    {
      EXPECT_EQ(game->GetHeader().Event, header.Event);
    }

    if (!header.Site.empty())
    {
      EXPECT_EQ(game->GetHeader().Site, header.Site);
    }

    if (game->GetHeader().Termination == "normal")
    {
      EXPECT_GT(game->GetHeader().PlyCount, 8);
      EXPECT_EQ(game->GetBoardHistory().moves.size(), game->GetHeader().PlyCount);
    }
  }

  return tournament;
}


