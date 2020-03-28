#pragma once

#include <string>
#include "types.h"


constexpr char Stockfish [] = "Stockfish";


struct TournamentHeader 
{

  std::string Event;
  std::string Site;

};

struct PGNHeader : TournamentHeader
{

  std::string Date;
  std::string Round;

  std::string WhitePlayer = "White";
  std::string BlackPlayer = "Black";

  int WhitePlayerELO{ 0 };
  int BlackPlayerELO{ 0 };

  std::string ECOCode;
  std::string Opening;
  std::string Variation;

  std::string FEN;

  std::string TimeControl;

  std::string Termination{ "normal" };

  int PlyCount{ 0 };
  int Result{ 0 };


  Color PlayerSide( std::string const &player ) const;

  inline bool HasPlayed(std::string const &player) const
  {
    return PlayerSide(player) != Color::COLOR_NB;
  }

  inline bool NoCrashEnding() const
  {
    return Termination == "normal" || 
      Termination == "abandoned" ||
      Termination == "adjudication";
  }

  std::string GetResult() const;

};

/// https://www.chessprogramming.org/Match_Statistics

struct GameStats
{

  GameStats() = default;

  GameStats(std::string const & p1, std::string const & p2) :
    Players(p1, p2)
  {
  }

  GameStats(std::string const & p1, std::string const & p2, int wins, int loss, int draw) :
    Players( p1, p2 ),
    Wins( wins),
    Losses( loss ),
    Draws( draw )
  {
  }

  std::string GetScoreString() const;

  GameStats InverseSide() const;

  double Score() const;

  int ScoreDifference() const;

  int N() const;

  double WinRatio() const;
  double DrawRatio() const;

  double ELO_Difference() const;

  double LikelyhoodOfSuperiority() const;

  std::pair<std::string, std::string> Players;

  int Wins{ 0 };
  int Losses{ 0 };
  int Draws{ 0 };

};


struct PlayerScore
{

  PlayerScore() = default;

  PlayerScore(std::string const &name ) :
    Name(name)
  {
  }

  PlayerScore(std::string const &name, double S) :
    Name(name),
    Score(S)
  {
  }

  std::string Name;
  double Score{ 0.0 };

};


class CrossTable
{
public:

  typedef std::vector<std::vector<GameStats>> TableType;

  CrossTable() = delete;

  CrossTable(std::vector<std::string> const &playerName);

  size_t IndexOf(std::string const &playerName) const;

  TableType const & Table() const;

  PlayerScore Score(std::string const &playerName) const;

  GameStats Statistics(std::string const &playerName) const;

  std::vector<PlayerScore> Scores() const;

  void AddScore(GameStats const &stats);

  void SortByELOScore(int baseElo);


private:

  TableType table;

};


