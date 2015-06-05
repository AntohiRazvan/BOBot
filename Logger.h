#pragma once
#include "Manager.h"
#include <BWAPI.h>
#include <string>
#include <functional>
#include <iostream>
#include <fstream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/basic_logger.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/attributes.hpp>
#include <boost/algorithm/string.hpp>

class Logger : public Manager
{
  boost::log::sources::logger _logger;
  boost::log::attribute_set::iterator _timeStamp;
  boost::log::attribute_set::iterator _action;
  boost::log::attribute_set::iterator _target;
  
  int _gamesPlayed = 0;
  int _gamesWon = 0;
  int _gamesLost = 0;
  float _winPercentage = 0.00f;
  int _winsVsTerran = 0;
  int _lossesVsTerran = 0;
  float _terranWinPercentage = 0.00f;
  int _winsVsProtoss = 0;
  int _lossesVsProtoss = 0;
  float _protossWinPercentage = 0.00f;
  int _winsVsZerg = 0;
  int _lossesVsZerg = 0;
  float _zergWinPercentage = 0.00f;
  std::string _path = "C:\\Facultate\\SCGames\\";

public:
  Logger();
  Logger(std::string filename);
  void Log(std::string _event, std::string target);
  void onUnitCreate(BWAPI::Unit unit);
  void onUnitComplete(BWAPI::Unit unit);
  void onUnitDestroy(BWAPI::Unit unit);
  void onEnemyUnitDestroy(BWAPI::Unit unit);
  void onEnemyUnitDiscover(BWAPI::Unit unit);
  void onEnd(bool isWinner);
};

