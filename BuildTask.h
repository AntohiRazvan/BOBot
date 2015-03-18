#pragma once
#include <BWAPI.h>

enum Progress{
  WAITING, WORKER_UNDERWAY, CONSTRUCTING, DONE
};

class BuildTask
{
private:
  BWAPI::Unit _builder;
  BWAPI::UnitType _building;
  BWAPI::TilePosition _position;
  int _mineralPrice;
  int _gasPrice;
  int _priority;
  Progress _progress;

public:
  BuildTask(BWAPI::Unit builder, BWAPI::UnitType building);
  void execute();
  int getMineralPrice();
  int getGasPrice();
  int getPriority() const;
  Progress getProgress();
};
