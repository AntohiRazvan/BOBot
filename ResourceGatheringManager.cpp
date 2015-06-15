#include "ResourceGatheringManager.h"

using namespace std;
using namespace BWAPI;
using namespace Filter;

ResourceGatheringManager::ResourceGatheringManager(Unit dropLocation)
{
  _dropLocation = dropLocation;
}

void ResourceGatheringManager::AddMineralGatherer(Unit unit)
{
  _mineralGatherers.push_back(unit);
}

void ResourceGatheringManager::AddGasGatherer(Unit unit)
{
  _gasGatherers.push_back(unit);
}

list<Unit> ResourceGatheringManager::GetMineralGatherers()
{
  return _mineralGatherers;
}

list<Unit> ResourceGatheringManager::GetGasGatherers()
{
  return _gasGatherers;
}

Unit ResourceGatheringManager::GetDropLocation()
{
  return _dropLocation;
}

void ResourceGatheringManager::update()
{
  if (_mineralGatherers.size() == 0)
  {
    return;
  }

  if ((_gasGatherers.size() < 3) && CanGatherGas())
  {
    _gasGatherers.push_back(_mineralGatherers.back());
    _mineralGatherers.pop_back();
  }

  for (auto u : _mineralGatherers)
  {
    if (!u->isGatheringMinerals())
    {
      u->gather(_dropLocation->getClosestUnit(IsMineralField));
    }
  }

  if (CanGatherGas())
  {
    for (auto u : _gasGatherers)
    {
      if (!u->isGatheringGas())
      {
        u->gather(_gasGeyser);
      }
    }
  }

  if (_lastChecked + _checkInterval < Broodwar->getFrameCount())
  {
    _lastChecked = Broodwar->getFrameCount();
    int workersNeeded = 0;
    auto units = Broodwar->getUnitsInRadius(_dropLocation->getPosition(), 250);
    for (auto unit : units)
    {
      if (unit->getType() == UnitTypes::Resource_Mineral_Field)
      {
        workersNeeded += 3;
      }
    }
    if (CanGatherGas())
    {
      workersNeeded += 3;
    }
    _workersNeeded = workersNeeded;
  }
}

void ResourceGatheringManager::onUnitDestroy(Unit unit)
{
  list<Unit>::iterator it = find(_mineralGatherers.begin(), _mineralGatherers.end(), unit);
  if (it != _mineralGatherers.end()) _mineralGatherers.erase(it);
  it = find(_gasGatherers.begin(), _gasGatherers.end(), unit);
  if (it != _gasGatherers.end()) _gasGatherers.erase(it);
}

bool ResourceGatheringManager::CanGatherGas()
{
  auto units = Broodwar->getUnitsInRadius(_dropLocation->getPosition(), 250);
  for (auto unit : units)
  {
    if (unit->getType() == UnitTypes::Protoss_Assimilator)
    {
      _gasGeyser = unit;
      return true;
    }
  }
  return false;
}

Unit ResourceGatheringManager::GetWorker()
{
  if (_mineralGatherers.size() == 0)
  {
    return nullptr;
  }

  Unit u = _mineralGatherers.back();
  _mineralGatherers.pop_back();
  return u;
}

int ResourceGatheringManager::WorkersNeeded()
{
  return _workersNeeded;
}
