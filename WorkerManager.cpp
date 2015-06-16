#include "WorkerManager.h"

using namespace std;
using namespace BWAPI;
using namespace Filter;

WorkerManager::WorkerManager(ProductionManager *pm)
{
  _workerCount = 0;
  _productionManager = pm;
  for (auto u : Broodwar->self()->getUnits())
  {
    if (u->getType().isWorker())
    {
      AddWorker(u);
      _workerCount++;
    }
  }
}

WorkerManager::~WorkerManager()
{
  for (auto base : _bases)
  {
    delete base;
  }
}

list<ResourceGatheringManager*> WorkerManager::GetAllBases()
{
  return _bases;
}

void WorkerManager::AddWorker(Unit u)
{
  list<Unit>::iterator it = find(_occupiedWorkerList.begin(), _occupiedWorkerList.end(), u);
  if (it != _occupiedWorkerList.end()) _occupiedWorkerList.erase(it);
  _unassignedWorkerList.push_front(u);
  if (_bases.size() != 0)
  {
    u->gather(_bases.back()->GetDropLocation()->getClosestUnit(IsMineralField));
  }
}

Unit WorkerManager::GetWorker()
{
  if (_unassignedWorkerList.size() != 0)
  {
    Unit worker = _unassignedWorkerList.front();
    _unassignedWorkerList.pop_front();
    _occupiedWorkerList.push_back(worker);
    return worker;
  }
  else
  {
    if (_bases.size() > 0)
    {
      auto it = _bases.begin();
      Unit worker;
      while ((worker = (*it)->GetWorker()) == nullptr)
      {
        if (it == _bases.end())
        {
          return nullptr;
        }
        it++;
      }
      _occupiedWorkerList.push_back(worker);
      return worker;
    }
    return nullptr;
  }
}

void WorkerManager::update()
{
  if ((_workerCount > (_bases.size() * 24)) || (_workerCount > 80))
  {
    _productionManager->HaltNexusProduction();
  }
  else
  {
    _productionManager->ResumeNexusProduction();
  }

  if (_unassignedWorkerList.size() > 0)
  {
    Unit unit = _unassignedWorkerList.front();
    for (auto base : _bases)
    {
      if (base->GetMineralGatherers().size() < base->WorkersNeeded())
      {
        base->AddMineralGatherer(unit);
        _unassignedWorkerList.pop_front();
        break;
      }
      else if (base->GetMineralGatherers().size() > base->WorkersNeeded())
      {
        int surplusWorkers = base->WorkersNeeded() - base->GetMineralGatherers().size();
        while (surplusWorkers > 0)
        {
          Unit worker = base->GetWorker();
          _unassignedWorkerList.push_back(worker);
        }
      }
    }
    if (unit->isIdle())
    {
      if (_bases.size() != 0)
      {
        unit->gather(_bases.back()->GetDropLocation()->getClosestUnit(IsMineralField));
      }
    }
  }

  for (auto base : _bases)
  {
    base->update();
  }
}

void WorkerManager::onUnitComplete(BWAPI::Unit unit)
{
  if (unit->getType().isWorker())
  {
    _workerCount++;
    AddWorker(unit);
  }
  if (unit->getType().isResourceDepot())
  {
    ResourceGatheringManager* base = new ResourceGatheringManager(unit);
    _bases.push_back(base);
  }
}

void WorkerManager::onUnitDestroy(BWAPI::Unit unit)
{
  if (!unit->getType().isWorker() || !unit->getType().isResourceDepot())
  {
    return;
  }
  if (unit->getType().isWorker()) _workerCount--;
  list<Unit>::iterator it = find(_occupiedWorkerList.begin(), _occupiedWorkerList.end(), unit);
  if (it != _occupiedWorkerList.end()) _occupiedWorkerList.erase(it);
  it = find(_unassignedWorkerList.begin(), _unassignedWorkerList.end(), unit);
  if (it != _unassignedWorkerList.end()) _unassignedWorkerList.erase(it);
  list<ResourceGatheringManager*>::iterator base = find_if(_bases.begin(), _bases.end(), 
                                                    [unit](ResourceGatheringManager* base)
                                                    { if (base->GetDropLocation() == unit) return true;
                                                      return false;
                                                    });
  if (base != _bases.end())
  {
    for (Unit worker : (*base)->GetMineralGatherers())
    {
      _unassignedWorkerList.push_back(worker);
    }
    for (Unit worker : (*base)->GetGasGatherers())
    {
      _unassignedWorkerList.push_back(worker);
    }
    _bases.erase(base);
  }
  for (auto base : _bases)
  {
    base->onUnitDestroy(unit);
  }
}
