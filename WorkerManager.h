#pragma once
#include <list>
#include <BWAPI.h>
#include "Manager.h"
#include "ResourceGatheringManager.h"

class WorkerManager : public Manager
{
  std::list<ResourceGatheringManager*> _bases;
  std::list<BWAPI::Unit> _unassignedWorkerList;
  std::list<BWAPI::Unit> _occupiedWorkerList;

public:
  WorkerManager();
  ~WorkerManager();
  void update();
  void onUnitComplete(BWAPI::Unit unit);
  void onUnitDestroy(BWAPI::Unit unit);
  void AddWorker(BWAPI::Unit u);
  BWAPI::Unit GetWorker();
  std::list<ResourceGatheringManager*> GetAllBases();
};
