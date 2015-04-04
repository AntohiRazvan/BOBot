#pragma once
#include <queue>
#include <vector>
#include <iterator>
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

class BuildingManager : Manager
{
private:
  std::priority_queue<BuildTask*, std::vector<BuildTask*>, CompareBuildTaskPriority> _buildQueue;
  std::vector<BuildTask*> _buildingsInProgress;
  std::map<BWAPI::UnitType, int> _buildingsMade;

  WorkerManager *_workerManager;

  int _minSupplyLeft = 8;
  bool _pylonInQueue = false;

  void FinishBuildingsCheck();
  void SupplyCheck();
  void StartBuildingCheck();
  void SendBuilders();

public:
  BuildingManager(WorkerManager *wm);
  void AddBuildRequest(BWAPI::UnitType building, Priority piriority = Priority::LOW);
  void Update();
};
