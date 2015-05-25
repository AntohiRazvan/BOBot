#include "BuildTask.h"

using namespace BWAPI;

BuildTask::BuildTask(Unit builder, UnitType building, WorkerManager *workerManager)
{
  _builder = builder;
  _building = building;
  _mineralPrice = building.mineralPrice();
  _gasPrice = building.gasPrice();
  _position = Broodwar->getBuildLocation(building, (TilePosition)_builder->getPosition(), 10);
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
  _position = Broodwar->getBuildLocation(building, (TilePosition)_builder->getPosition(), 10);
  _progress = Progress::WAITING;
  _priority = priority;
  _startTime = 0;
  _workerManager = workerManager;
}

void BuildTask::StartBuidling()
{
  if (!_builder)
  {
    _builder = _workerManager->GetWorker();
  }
  _builder->build(_building, _position);
  _startTime = Broodwar->getFrameCount();
}

void BuildTask::SendBuilder()
{
  if (_position == INVALID_POSITION)
  {
    _position = Broodwar->getBuildLocation(_building, (TilePosition)_builder->getPosition(), 10);
  }
  if (_progress == Progress::WAITING)
  {
    _builder->move(Position(_position));
    _progress = Progress::WORKER_UNDERWAY;
  }
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
  if ((_builder->getTilePosition().getDistance(_position) < 10) &&
      (_progress == Progress::WORKER_UNDERWAY))
  {
    _progress = Progress::WORKER_ARRIVED;
  }

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

void BuildTask::FreeWorker()
{
  _workerManager->AddWorker(_builder);
}

