#pragma once
#include "Manager.h"
#include <BWAPI.h>
#include <algorithm>
#include <list>

class ResourceGatheringManager
{
  int _workersNeeded;
  int _checkInterval = 120;
  int _lastChecked = 0;
  BWAPI::Unit _dropLocation;
  std::list<BWAPI::Unit> _mineralGatherers;
  std::list<BWAPI::Unit> _gasGatherers;
  bool CanGatherGas();
  BWAPI::Unit _gasGeyser;

public:
  ResourceGatheringManager(BWAPI::Unit dropLocation);

  BWAPI::Unit GetDropLocation();
  void AddMineralGatherer(BWAPI::Unit unit);
  void AddGasGatherer(BWAPI::Unit unit);
  std::list<BWAPI::Unit> GetMineralGatherers();
  std::list<BWAPI::Unit> GetGasGatherers();
  BWAPI::Unit GetWorker();
  int WorkersNeeded();

  void update();
  void onUnitDestroy(BWAPI::Unit unit);
};
