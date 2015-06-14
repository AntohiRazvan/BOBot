#pragma once
#include <BWAPI.h>
#include <vector>
#include <queue>
#include <algorithm>
#include "Manager.h"
#include "TerrainAnalyzer.h"
#include "WorkerManager.h"


class ScoutManager : public Manager
{
  WorkerManager *_workerManager;
  TerrainAnalyzer *_terrainAnalyzer;
  BWAPI::Unit _scout;
  std::vector<BWAPI::Position> _enemyBaseLocations;
  std::vector<BWAPI::Unit> _enemyBuildings;
  std::queue<BWAPI::Position> _locationsToScout;
  int _scoutFrequency = 10000;
  int _lastScoutTime = -(_scoutFrequency - 2000);

  bool BaseIsMine(BWTA::BaseLocation* baseLocation);
  void Scout();
  void InitialScout();

public:
  ScoutManager(WorkerManager *wm);
  std::vector<BWAPI::Unit> GetEnemyBuildings();
  std::vector<BWAPI::Position> GetEnemyBaseLocations();

  void update();
  void onUnitDestroy(BWAPI::Unit unit);
  void onEnemyUnitDiscover(BWAPI::Unit unit);
};

