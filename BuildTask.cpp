#include "BuildTask.h"

using namespace BWAPI;

BuildTask::BuildTask(Unit builder, UnitType building, WorkerManager *workerManager)
{
  _builder = builder;
  _building = building;
  _mineralPrice = building.mineralPrice();
  _gasPrice = building.gasPrice();
  _position = Broodwar->getBuildLocation(building, TilePosition(_builder->getPosition()), 15);
  _progress = Progress::WAITING;
  _priority = Priority::LOW;
  _startTime = 0;
  _workerManager = workerManager;
}

BuildTask::BuildTask(Unit builder, UnitType building, WorkerManager *workerManager, Priority priority)
{
  _builder = builder;
  _building = building;
  _mineralPrice = building.mineralPrice();
  _gasPrice = building.gasPrice();
  _position = Broodwar->getBuildLocation(building, TilePosition(_builder->getPosition()), 15);
  _progress = Progress::WAITING;
  _priority = priority;
  _startTime = 0;
  _workerManager = workerManager;
}

void BuildTask::StartBuidling()
{
  _builder->build(_building, _position);
  if ((Broodwar->getLastError() == Errors::Insufficient_Minerals) ||
      (Broodwar->getLastError() == Errors::Insufficient_Gas))
  {
    return;
  }
  _hasStarted = true;
  _progress = Progress::BUILDING;
  _startTime = Broodwar->getFrameCount();
  _workerManager->AddWorker(_builder);

}

void BuildTask::SendBuilder()
{
  _builder->move(Position(_position));
  _progress = Progress::WORKER_UNDERWAY;
}

bool BuildTask::WorkerArrived()
{
  if (_builder->getTilePosition().getDistance(_position) < 10)
    return true;
  return false;
}

int BuildTask::GetMineralPrice()
{
  return _mineralPrice;
}

int BuildTask::GetGasPrice()
{
  return _gasPrice;
}

Priority BuildTask::GetPriority() const
{
  return _priority;
}
 
Progress BuildTask::GetProgress()
{
  return _progress;
}

UnitType BuildTask::GetBuildingType()
{
  return _building;
}

void BuildTask::SetProgress(Progress progress)
{
  _progress = progress;
}

int BuildTask::GetStartTime()
{
  return _startTime;
}

bool BuildTask::HasStarted()
{
  return _hasStarted;
}