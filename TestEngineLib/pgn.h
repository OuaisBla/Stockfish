#pragma once

#include "BoardHistory.h"
#include "MoveStats.h"
#include "PGNHeader.h"
#include "EvalStacker.h"
#include <list>
#include <memory>


struct PGNGame {

  PGNGame() = default;
  PGNGame(PGNGame const &) = delete;

  ~PGNGame() = default;

  PGNHeader const & GetHeader() const;
  void SetHeader(PGNHeader const &);

  BoardHistory const &GetBoardHistory() const;

  BoardHistory &GetBoardHistory();

  EvalStacker EvalGraph() const;

private:

  PGNHeader header;

  BoardHistory bh;

};


class PGNTournament
{
public:

  TournamentHeader const &Header() const;

  void AddGame(std::shared_ptr<PGNGame> game);

  void AddGames(std::vector<std::shared_ptr<PGNGame>> const &games);

  std::vector<std::shared_ptr<PGNGame>> const & GetGames() const;

  std::vector<std::shared_ptr<PGNGame>> GetNoCrashGames() const;
  
  // Return the list of games that match non empty PGNHeader values;
  std::vector<std::shared_ptr<PGNGame>> GetGames( std::string const & ) const;

  std::vector<std::string> GetPlayerNames() const;

  GameStats GetStatistics(std::string const &player1, std::string const &player2) const;

  CrossTable GetCrossTable() const;

  std::vector<PlayerScore> ELOScores(int baseElo, int K) const;

  template <typename Predicate>
  std::string GetCrossTable(Predicate pred) const;

  std::string GetWinCrossTable() const;
  std::string GetLossCrossTable() const;
  std::string GetDrawCrossTable() const;

private:

  std::vector<std::shared_ptr<PGNGame> > games;

  TournamentHeader header{ "Database", "Local" };

};


class PGNParser {
 public:

  static const char* StartFEN;

  static std::unique_ptr<PGNTournament> parse( std::string const& file);

  template<bool Strict = true>
  static bool move_is_san(Position const& pos, Move m, const char* ref);

  static std::string move_to_san(Position const& pos, Move m);
  static Move san_to_move(Position const& pos, const std::string& s);


 private:

   static std::unique_ptr<PGNGame> parse(std::istream &is_);

   static PGNHeader parseHeader(std::istream &is_);

};

