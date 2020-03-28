#include "pch.h"
#include <gtest/gtest.h>
#include "pgn.h"
#include "EvalStacker.h"
#include "EngineInit.h"
#include "TrendDetector.h"
#include <iostream>

using namespace std;


class TrendDetectorTest : public ::testing::Test {
protected:
  static void SetUpTestCase() {

    EngineInit();

  }

  static void TearDownTestCase()
  {

    EngineReset();

  }

};

TEST_F(TrendDetectorTest, TrendTest_ccc_1_stage_1_games) {

  auto tournament = PGNParser::parse("ccc_1_stage_1_games.pgn");

  for (auto game : tournament->GetGames(Stockfish))
  {
    cout << game->GetHeader().WhitePlayer << " - " << game->GetHeader().BlackPlayer << " (" << game->GetHeader().PlyCount << ") => " << game->GetHeader().GetResult() << endl;

    EvalStacker graph = game->EvalGraph();

    TrendDetector white(graph.GetWhiteMoves(), graph.GetWhiteEvalCurve());

    TrendDetector black(graph.GetBlackMoves(), graph.GetBlackEvalCurve());


    cout << "W-Side: " << (white.FavorableSide() == WHITE ? "W" : "B") << endl;
    cout << "B-Side: " << (black.FavorableSide() == WHITE ? "W" : "B") << endl;

    {
      auto const &coeff = white.GetPolynomial3Coefficients();

      cout << "W-Coeff: " << coeff[3] << " x3 + " << coeff[2] << " x2 + " << coeff[1] << " x + " << coeff[0] << " = 0" << endl;
    }

    {
      auto const &coeff = black.GetPolynomial3Coefficients();

      cout << "B-Coeff: " << coeff[3] << " x3 + " << coeff[2] << " x2 + " << coeff[1] << " x + " << coeff[0] << " = 0" << endl;
    }

    {
      std::pair<double, double> coeff = white.GetExponentialCoefficients();

      cout << "W-Exp coeffs: a=" << coeff.first << ", b=" << coeff.second << endl;
    }

    {
      std::pair<double, double> coeff = black.GetExponentialCoefficients();

      cout << "B-Exp coeffs: a=" << coeff.first << ", b=" << coeff.second << endl;
    }

    {
      std::pair<double, double> coeff = white.GetLogarithmCoefficients();

      cout << "W-Log coeffs: a=" << coeff.first << ", b=" << coeff.second << endl;
    }

    {
      std::pair<double, double> coeff = black.GetLogarithmCoefficients();

      cout << "B-Log coeffs: a=" << coeff.first << ", b=" << coeff.second << endl;
    }


  }

}
