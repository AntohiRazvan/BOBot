#include "Logger.h"

using namespace BWAPI;
using namespace std;
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace attrs = boost::log::attributes;
using namespace logging::trivial;

int GetFrameCount()
{
  return Broodwar->getFrameCount();
}

Logger::Logger()
{
  logging::add_file_log
    (
    keywords::file_name = "log_%N.log",
    keywords::format = "[%TimeStamp%][%Action%][%Target%]"
    );
  _timeStamp = _logger.add_attribute("TimeStamp", attrs::function<int>(&GetFrameCount)).first;
 
}

Logger::Logger(string filename)
{
  logging::add_file_log
    (
    keywords::file_name = "filename",
    keywords::format = "[%TimeStamp%][%Action%][%Target%]"
    );
  _timeStamp = _logger.add_attribute("TimeStamp", attrs::function<int>(&GetFrameCount)).first;

}

void Logger::Log(string action, string target)
{
  _action = _logger.add_attribute("Action", attrs::constant<string>(action)).first;
  _target = _logger.add_attribute("Target", attrs::constant<string>(target)).first;
  boost::log::attribute_set::iterator _timeStamp;
  BOOST_LOG(_logger) << "";
  _logger.remove_attribute(_action);
  _logger.remove_attribute(_target);
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
  if (isWinner)
    Log("I", "won");
  else
    Log("I", "lost");
}


