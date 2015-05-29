#pragma once
#include "Manager.h"
#include <BWAPI.h>
#include <string>
#include "boost\log\core.hpp"
#include "boost\log\trivial.hpp"
#include "boost\log\expressions.hpp"
#include "boost\log\sinks\text_file_backend.hpp"
#include "boost\log\utility\setup\file.hpp"
#include "boost\log\utility\setup\common_attributes.hpp"
#include "boost\log\sources\record_ostream.hpp"
#include "boost\log\sources\basic_logger.hpp"
#include "boost\log\sources\logger.hpp"
#include "boost\log\attributes.hpp"

class Logger : public Manager
{
  boost::log::sources::logger _logger;
  boost::log::attribute_set::iterator _timeStamp;
  boost::log::attribute_set::iterator _action;
  boost::log::attribute_set::iterator _target;

public:
  Logger();
  void Log(std::string _event, std::string target);
  void onUnitCreate(BWAPI::Unit unit);
  void onUnitComplete(BWAPI::Unit unit);
  void onUnitDestroy(BWAPI::Unit unit);
};

