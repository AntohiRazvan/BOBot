#pragma once
#include <queue>
#include <vector>
#include <iterator>
#include <map>
#include "BuildTask.h"
#include "IManager.h"

#define MAX_SUPPLY 200

struct CompareBuildTaskPriority 
{
  bool operator()(const BuildTask* BuildTask1, const BuildTask* BuildTask2)
  {
    return BuildTask1->GetPriority() > BuildTask2->GetPriority();
  }
};

class BuildingManager : IManager
{
private:
  std::priority_queue<BuildTask*, std::vector<BuildTask*>, CompareBuildTaskPriority> _buildQueue;
  std::vector<BuildTask*> _buildingsInProgress;
  std::map<BWAPI::UnitType, int> _buildingsMade;

  int minSupplyLeft = 4;
  bool pylonInQueue = false;

  void FinishBuildingsCheck();
  void SupplyCheck();
  void StartBuildingCheck();
  void SendBuilders();

public:
  BuildingManager();
  void AddBuildRequest(BWAPI::UnitType building, Priority piriority = Priority::LOW);
  void Update();
};
