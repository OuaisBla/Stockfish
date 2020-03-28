#include "stdafx.h"
#include "PGNHeader.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

Color PGNHeader::PlayerSide(std::string const &player) const {

  if (WhitePlayer.substr(0, player.size() ) == player)
  {
    return WHITE;
  }

  if (BlackPlayer.substr(0, player.size()) == player)
  {
    return BLACK;
  }

  return Color::COLOR_NB;
}

std::string PGNHeader::GetResult() const
{
  if (Result == 1) {
    return "1-0";
  }
  else if (Result == -1) {
    return "0-1";
  }
  else if (Result == 0 && NoCrashEnding()) {
    return "1/2-1/2";
  }

  return "*";
}

std::string GameStats::GetScoreString() const
{
  return "+" + std::to_string(Wins) + " -" + std::to_string(Losses) + " =" + std::to_string(Draws);
}


GameStats GameStats::InverseSide() const
{
  return GameStats(Players.second, Players.first, Losses, Wins, Draws);
}

double GameStats::Score() const {
  return Wins + (double(Draws) / 2.0);
}


int GameStats::ScoreDifference() const {
  return Wins - Losses;
}


int GameStats::N() const {
  return Wins + Losses + Draws;
}


double GameStats::WinRatio() const
{
  return Score() / N();
}

double GameStats::DrawRatio() const
{
  return double( Draws ) / N();
}

double GameStats::ELO_Difference() const
{
  double winning_fraction = double(Wins + 0.5 * Draws) / N();

  return -std::log(1.0 / winning_fraction - 1.0) * 400.0 / std::log(10.0);
}


double GameStats::LikelyhoodOfSuperiority() const
{
  return .5 + .5 * std::erf( double( ScoreDifference() ) / std::sqrt( 2.0 * (Wins + Losses) ) );
}



CrossTable::CrossTable(std::vector<std::string> const &playerName)
{
  for (size_t i = 0; i < playerName.size(); ++i)
  {
    std::vector<GameStats> stats(playerName.size());

    for (size_t j = 0; j < playerName.size(); ++j)
    {
      stats[j] = GameStats(playerName[i], playerName[j]);
    }

    table.push_back( stats );
  }
}

void CrossTable::AddScore(GameStats const &stats)
{
  size_t const index1 = IndexOf(stats.Players.first);
  size_t const index2 = IndexOf(stats.Players.second);

  if (index1 == -1)
  {
    throw std::runtime_error(stats.Players.first + " is not in the cross table.");
  }

  if (index2 == -1)
  {
    throw std::runtime_error(stats.Players.second + " is not in the cross table.");
  }

  table[index1][index2] = stats;
  table[index2][index1] = stats.InverseSide();
}

void CrossTable::SortByELOScore(int baseElo)
{
  
}

size_t CrossTable::IndexOf(std::string const &playerName) const
{
  if(table.empty())
  {
    return -1;
  }

  TableType::const_iterator i = std::find_if(table.cbegin(), table.cend(), [playerName](std::vector<GameStats> const &s) { return playerName == s[0].Players.first; });

  if (i == table.cend())
  {
    return -1;
  }

  return i - table.cbegin();
}

CrossTable::TableType const &CrossTable::Table() const
{
  return table;
}

std::vector<PlayerScore> CrossTable::Scores() const
{
  std::vector<PlayerScore> scores;

  if (table.empty())
  {
    return scores;
  }

  for (size_t i = 0; i < table.size(); i++)
  {
    PlayerScore ps( table[i][0].Players.first );

    for (size_t j = 0; j < table.size(); j++)
    {
      ps.Score += table[i][j].Score();
    }

    scores.push_back(ps);
  }

  return scores;
}

PlayerScore CrossTable::Score(std::string const &playerName) const
{
  size_t const index1 = IndexOf(playerName);

  if (index1 == -1)
  {
    throw std::runtime_error(playerName + " is not in the cross table.");
  }

  PlayerScore ps( playerName );

  for (size_t i = 0; i < table.size(); i++)
  {
    ps.Score += table[index1][i].Score();
  }

  return ps;
}

GameStats CrossTable::Statistics(std::string const &playerName) const
{
  size_t const index1 = IndexOf(playerName);

  if (index1 == -1)
  {
    throw std::runtime_error(playerName + " is not in the cross table.");
  }

  GameStats gs(playerName, "");

  for (size_t i = 0; i < table.size(); i++)
  {
    gs.Wins += table[index1][i].Wins;
    gs.Losses += table[index1][i].Losses;
    gs.Draws += table[index1][i].Draws;
  }

  return gs;
}

