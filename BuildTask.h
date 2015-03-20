#pragma once
#include <BWAPI.h>
#include "WorkerManager.h"
enum Progress 
{
  WAITING, WORKER_UNDERWAY, BUILDING
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
  BWAPI::TilePosition _position;
  int _mineralPrice;
  int _gasPrice;
  int _startTime;
  bool _hasStarted = false;
  Priority _priority;
  Progress _progress;
  WorkerManager *_workerManager;

public:
  BuildTask(BWAPI::Unit builder, BWAPI::UnitType building, WorkerManager *workerManager);
  BuildTask(BWAPI::Unit builder, BWAPI::UnitType building, WorkerManager *workerManager, Priority priority);
  void StartBuidling();
  void SendBuilder();
  int GetMineralPrice();
  int GetGasPrice();
  int GetStartTime();
  BWAPI::UnitType GetBuildingType();
  Priority GetPriority() const;
  Progress GetProgress();
  void SetProgress(Progress progress);
  bool WorkerArrived();
  bool IsFinished();
  bool HasStarted();
};
