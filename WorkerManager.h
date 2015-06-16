#pragma once
#include <list>
#include <BWAPI.h>
#include "Manager.h"
#include "ResourceGatheringManager.h"
#include "ProductionManager.h"

class WorkerManager : public Manager
{
  ProductionManager *_productionManager;
  std::list<ResourceGatheringManager*> _bases;
  std::list<BWAPI::Unit> _unassignedWorkerList;
  std::list<BWAPI::Unit> _occupiedWorkerList;
  int _workerCount;

public:
  WorkerManager(ProductionManager *pm);
  ~WorkerManager();
  void update();
  void onUnitComplete(BWAPI::Unit unit);
  void onUnitDestroy(BWAPI::Unit unit);
  void AddWorker(BWAPI::Unit u);
  BWAPI::Unit GetWorker();
  std::list<ResourceGatheringManager*> GetAllBases();

};
