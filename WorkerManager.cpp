#include "WorkerManager.h"

using namespace std;
using namespace BWAPI;
using namespace Filter;

WorkerManager::WorkerManager()
{
  for (auto u : Broodwar->self()->getUnits())
  {
    if (u->getType() == UnitTypes::Protoss_Probe)
    {
      AddWorker(u);
    }
  }
}

void WorkerManager::AddWorker(Unit u)
{
  list<Unit>::iterator it = find(_buildingWorkerList.begin(), _buildingWorkerList.end(), u);
  if (it != _buildingWorkerList.end()) _buildingWorkerList.erase(it);
  _gatheringWorkerList.push_back(u);
  u->gather(u->getClosestUnit(IsMineralField));
}

Unit WorkerManager::GetWorker()
{
  if (!_gatheringWorkerList.empty())
  {
    Unit u = _gatheringWorkerList.front();
    _gatheringWorkerList.pop_front();
    _buildingWorkerList.push_back(u);
    return u;
  }
  return nullptr;
}

void WorkerManager::update()
{
  for (auto u : _gatheringWorkerList)
  {
    if (u->isIdle())
    {
      u->gather(u->getClosestUnit(IsMineralField));
    }
  }
}

void WorkerManager::onUnitComplete(BWAPI::Unit unit)
{
  if (unit->getType().isWorker())
  {
    AddWorker(unit);
  }
}

void WorkerManager::onUnitDestroy(BWAPI::Unit unit)
{
  list<Unit>::iterator it = find(_buildingWorkerList.begin(), _buildingWorkerList.end(), unit);
  if (it != _buildingWorkerList.end()) _buildingWorkerList.erase(it);
  it = find(_gatheringWorkerList.begin(), _gatheringWorkerList.end(), unit);
  if (it != _gatheringWorkerList.end()) _gatheringWorkerList.erase(it);
}
