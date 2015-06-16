#include "BuildTask.h"

using namespace BWAPI;
using namespace BWTA;

BuildTask::BuildTask(UnitType building, WorkerManager *workerManager)
{
  _building = building;
  _mineralPrice = building.mineralPrice();
  _gasPrice = building.gasPrice();
  _progress = Progress::WAITING;
  _priority = Priority::MEDIUM;
  _startTime = 0;
  _workerManager = workerManager;
}

BuildTask::BuildTask(UnitType building, WorkerManager *workerManager, Priority priority)
{
  _building = building;
  _mineralPrice = building.mineralPrice();
  _gasPrice = building.gasPrice();
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
  if (!Broodwar->canBuildHere(_position, _building))
  {
    _position = Broodwar->getBuildLocation(_building, (TilePosition)_builder->getPosition(), 50);
  }
  _builder->build(_building, _position);
  _startTime = Broodwar->getFrameCount();
}

void BuildTask::SendBuilder()
{
  _builder = _workerManager->GetWorker();
  if (_builder)
  {
    _position = Broodwar->getBuildLocation(_building, (TilePosition)_builder->getPosition(), 20);
  }
  if (_building.isResourceDepot())
  {
      TerrainAnalyzer *ta = TerrainAnalyzer::Instance();
      BaseLocation *baseLocation = ta->GetNearestBaseLocation(_workerManager->GetAllBases().back()->GetDropLocation()->getPosition());
      _position = ta->GetNearestExpansionLocation(baseLocation);
  }
  else
  {
    if (!Broodwar->canBuildHere(_position, _building))
    {
      _position = Broodwar->getBuildLocation(_building, (TilePosition)_builder->getPosition(), 50);
    }
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
  if ((_progress == Progress::WORKER_UNDERWAY) &&
      (_builder->getTilePosition().getApproxDistance(_position) < 10))
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
