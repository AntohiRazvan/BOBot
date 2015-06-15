#pragma once
#include <BWAPI.h>
#include "WorkerManager.h"
#include "TerrainAnalyzer.h"

enum Progress 
{
  WAITING, WORKER_UNDERWAY, WORKER_ARRIVED, BUILDING
};

enum Priority 
{
  LOW, MEDIUM, HIGH
};

class BuildTask 
{
private:
  BWAPI::Unit _builder;
  BWAPI::UnitType _building;
  int _mineralPrice;
  int _gasPrice;
  int _startTime;
  bool _buildingPlaced;
  Priority _priority;
  Progress _progress;
  WorkerManager *_workerManager;

public:
  BWAPI::TilePosition _position;

  BuildTask(BWAPI::UnitType building, WorkerManager *workerManager);
  BuildTask(BWAPI::UnitType building, WorkerManager *workerManager, Priority priority);
  void StartBuidling();
  void SendBuilder();
  int GetMineralPrice();
  int GetGasPrice();
  int GetStartTime();
  BWAPI::UnitType GetBuildingType();
  Priority GetPriority() const;
  Progress GetProgress();
  void SetProgress(Progress progress);
  void FreeWorker();
};
