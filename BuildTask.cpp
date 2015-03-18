#include "BuildTask.h"

using namespace BWAPI;

BuildTask::BuildTask(Unit builder, UnitType building)
{
  _builder = builder;
  _building = building;
  _mineralPrice = building.mineralPrice();
  _gasPrice = building.gasPrice();
  _position = Broodwar->getBuildLocation(building, TilePosition(_builder->getPosition()), 15);
  _progress = Progress::WAITING;
}

void BuildTask::execute()
{
  _builder->build(_building, _position);
}

int BuildTask::getMineralPrice()
{
  return _mineralPrice;
}

int BuildTask::getGasPrice()
{
  return _gasPrice;
}

int BuildTask::getPriority() const
{
  return _priority;
}

Progress BuildTask::getProgress()
{
  return _progress;
}
