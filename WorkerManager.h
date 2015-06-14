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
  void update();
  void onUnitComplete(BWAPI::Unit unit);
  void onUnitDestroy(BWAPI::Unit unit);
  void AddWorker(BWAPI::Unit u);
  BWAPI::Unit GetWorker();
};

