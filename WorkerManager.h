#pragma once
#include <list>
#include <BWAPI.h>
#include "Manager.h"

class WorkerManager : public Manager
{
  std::list<BWAPI::Unit> _gatheringWorkerList;
  std::list<BWAPI::Unit> _buildingWorkerList;
public:
  WorkerManager();
  void Update();
  void onUnitComplete(BWAPI::Unit unit);
  void onUnitCreate(BWAPI::Unit unit);
  void AddWorker(BWAPI::Unit u);
  BWAPI::Unit GetWorker();
};

