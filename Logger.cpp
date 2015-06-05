#include "Logger.h"

#define STRIP_TEXT temp >> temp >> temp

using namespace BWAPI;
using namespace std;
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace attrs = boost::log::attributes;
using namespace logging::trivial;

Logger::Logger()
{
  ifstream statsFile(_path+"Statistics.txt");
  if (statsFile.good())
  {
    string temp;
    statsFile >> temp >> temp >> _gamesPlayed;
    statsFile >> temp >> temp >> _gamesWon >> temp >> temp >> _gamesLost >> temp >> temp >> _winPercentage >> temp;
    statsFile >> STRIP_TEXT >> _winsVsTerran >> STRIP_TEXT >> _lossesVsTerran >> temp >> temp >> _terranWinPercentage >> temp;
    statsFile >> STRIP_TEXT >> _winsVsProtoss >> STRIP_TEXT >> _lossesVsProtoss >> temp >> temp >> _protossWinPercentage >> temp;
    statsFile >> STRIP_TEXT >> _winsVsZerg >> STRIP_TEXT >> _lossesVsZerg >> temp >> temp >> _zergWinPercentage >> temp;
  }
  statsFile.close();

  logging::add_file_log
    (
    keywords::file_name = _path + "log_" + to_string(_gamesPlayed) + ".log",
    keywords::format = "[%TimeStamp%][%Action%][%Target%]"
    );
    _timeStamp = _logger.add_attribute("TimeStamp", attrs::function<int>([](){return Broodwar->getFrameCount();})).first;

}

Logger::Logger(string filename)
{
  ifstream statsFile(_path + "Statistics.txt");
  if (statsFile.good())
  {
    string temp;
    statsFile >> temp >> temp >> _gamesPlayed;
    statsFile >> temp >> temp >> _gamesWon >> temp >> temp >> _gamesLost >> temp >> temp >> _winPercentage >> temp;
    statsFile >> STRIP_TEXT >> _winsVsTerran >> STRIP_TEXT >> _lossesVsTerran >> temp >> temp >> _terranWinPercentage >> temp;
    statsFile >> STRIP_TEXT >> _winsVsProtoss >> STRIP_TEXT >> _lossesVsProtoss >> temp >> temp >> _protossWinPercentage >> temp;
    statsFile >> STRIP_TEXT >> _winsVsZerg >> STRIP_TEXT >> _lossesVsZerg >> temp >> temp >> _zergWinPercentage >> temp;
  }
  statsFile.close();

  logging::add_file_log
    (
    keywords::file_name = filename + to_string(_gamesPlayed) + ".log",
    keywords::format = "[%TimeStamp%][%Action%][%Target%]"
    );
  _timeStamp = _logger.add_attribute("TimeStamp", attrs::function<int>([](){return Broodwar->getFrameCount(); })).first;
}

void Logger::Log(string action, string target)
{
  if (Broodwar->getFrameCount() > 1)
  {
    _action = _logger.add_attribute("Action", attrs::constant<string>(action)).first;
    _target = _logger.add_attribute("Target", attrs::constant<string>(target)).first;
    boost::log::attribute_set::iterator _timeStamp;
    BOOST_LOG(_logger) << "";
    _logger.remove_attribute(_action);
    _logger.remove_attribute(_target);
  }
}

void Logger::onUnitCreate(Unit unit) 
{
  if (unit->getPlayer() == BWAPI::Broodwar->self())
  {
    Log("Created", unit->getType().getName());
  }
}

void Logger::onUnitComplete(Unit unit)
{
  if (unit->getPlayer() == BWAPI::Broodwar->self())
  {
    Log("Completed", unit->getType().getName());
  }
}

void Logger::onUnitDestroy(Unit unit)
{
    Log("Lost", unit->getType().getName());
}

void Logger::onEnemyUnitDestroy(Unit unit)
{
  Log("Killed", unit->getType().getName());
}

void Logger::onEnemyUnitDiscover(Unit unit)
{
  Log("Discovered", unit->getType().getName());
}

void Logger::onEnd(bool isWinner)
{
  _gamesPlayed++;
  if (isWinner)
  {
    Log("Won", "Game");
    _gamesWon++;
    if (Broodwar->enemy()->getRace() == Races::Terran)
    {
      _winsVsTerran++;
    }
    else if (Broodwar->enemy()->getRace() == Races::Protoss)
    {
      _winsVsProtoss++;
    }
    else if (Broodwar->enemy()->getRace() == Races::Zerg)
    {
      _winsVsZerg++;
    }
  }
  else
  {
    Log("Lost", "Game");
    _gamesLost++;
    if (Broodwar->enemy()->getRace() == Races::Terran)
    {
      _lossesVsTerran++;
    }
    else if (Broodwar->enemy()->getRace() == Races::Protoss)
    {
      _lossesVsProtoss++;
    }
    else if (Broodwar->enemy()->getRace() == Races::Zerg)
    {
      _lossesVsZerg++;
    }
  }

  _winPercentage = (float)_gamesWon / (float)_gamesPlayed * 100;
  if (_winsVsTerran + _lossesVsTerran > 0)
  {
    _terranWinPercentage = (float)_winsVsTerran / (float)(_winsVsTerran + _lossesVsTerran) * 100;
  }
  if (_winsVsProtoss + _lossesVsProtoss > 0)
  {
    _protossWinPercentage = (float)_winsVsProtoss / (float)(_winsVsProtoss + _lossesVsProtoss) * 100;
  }  if (_winsVsZerg + _lossesVsZerg > 0)
  {
    _zergWinPercentage = (float)_winsVsZerg / (float)(_winsVsZerg + _lossesVsZerg) * 100;
  }

  ofstream statsFile(_path + "Statistics.txt");

  statsFile << "Games Played: " << _gamesPlayed << endl;
  statsFile << "Games Won: " << _gamesWon << " Games Lost: " << _gamesLost << " Win Percentage: " << _winPercentage << "%" << endl;
  statsFile << "Wins vs Terran: " << _winsVsTerran << " Losses vs Terran: " << _lossesVsTerran << " Win Percentage: " << _terranWinPercentage << "%" << endl;
  statsFile << "Wins vs Protoss: " << _winsVsProtoss << " Losses vs Protoss: " << _lossesVsProtoss << " Win Percentage: " << _protossWinPercentage << "%" << endl;
  statsFile << "Wins vs Zerg: " << _winsVsZerg << " Losses vs Zerg: " << _lossesVsZerg << " Win Percentage: " << _zergWinPercentage << "%" << endl;

  statsFile.close();
}
