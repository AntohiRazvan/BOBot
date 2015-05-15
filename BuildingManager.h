#pragma once
#include <queue>
#include <vector>
#include <iterator>
#include <list>
#include <map>
#include "WorkerManager.h"
#include "BuildTask.h"
#include "Manager.h"

#define MAX_SUPPLY 400

struct CompareBuildTaskPriority 
{
  bool operator()(const BuildTask* BuildTask1, const BuildTask* BuildTask2)
  {
    return BuildTask1->GetPriority() > BuildTask2->GetPriority();
  }
};

class BuildingManager : public Manager
{
private:
  std::priority_queue<BuildTask*, std::vector<BuildTask*>, CompareBuildTaskPriority> _buildingsInQueue;
  std::list<BuildTask*> _buildingsInProgress;
  std::map<BWAPI::UnitType, int> _buildingsMade;

  WorkerManager *_workerManager;

  int _minSupplyLeft = 8;
  bool _pylonInQueue = false;

  void SupplyCheck();
  void SendBuilders();
  void StartBuilding();

public:
  BuildingManager(WorkerManager *wm);
  void AddBuildRequest(BWAPI::UnitType building, Priority piriority = Priority::LOW);
  void Update();
  void onUnitComplete(BWAPI::Unit unit);
  void onUnitCreate(BWAPI::Unit unit);
};
