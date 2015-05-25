#include "BuildingManager.h"

using namespace BWAPI;
using namespace Filter;
using namespace std;

BuildingManager::BuildingManager(WorkerManager *wm, ResourceManager *rm)
{
  _workerManager = wm;
  _resourceManager = rm;
}

void BuildingManager::AddBuildRequest(UnitType building, Priority priority)
{
  Unit builder = _workerManager->GetWorker();
  BuildTask *buildTask = new BuildTask(builder, building, _workerManager, priority);
  _buildingsInQueue.push(buildTask);
}

void BuildingManager::Update()
{
  SupplyCheck();
  SendBuilders();
  StartBuilding();
}

void BuildingManager::SupplyCheck()
{
  if ((Broodwar->self()->supplyTotal() < MAX_SUPPLY) &&
      ((Broodwar->self()->supplyTotal() - Broodwar->self()->supplyUsed()) < _minSupplyLeft) &&
      (!_pylonInQueue))
  {
    AddBuildRequest(UnitTypes::Protoss_Pylon, Priority::HIGH);
    _pylonInQueue = true;
  }
}

void BuildingManager::SendBuilders()
{
  if (!_buildingsInQueue.empty())
  {
    int currentMinerals = Broodwar->self()->minerals();
    int currentGas = Broodwar->self()->gas();
    
    BuildTask* bt = _buildingsInQueue.top();
    if ((bt->GetMineralPrice() <= (currentMinerals - _resourceManager->GetReservedMinerals())) && 
      (bt->GetGasPrice() <= (currentGas - _resourceManager->GetReservedGas())))
    {
      _resourceManager->ReserveMinerals(bt->GetMineralPrice());
      _resourceManager->ReserveGas(bt->GetGasPrice());
      bt->SendBuilder();
    }
  }
}

void BuildingManager::StartBuilding()
{
  if (!_buildingsInQueue.empty())
  {
    BuildTask* bt = _buildingsInQueue.top();
    if (bt->GetProgress() == Progress::WORKER_ARRIVED)
    {
      int currentMinerals = Broodwar->self()->minerals();
      int currentGas = Broodwar->self()->gas();

      if ((bt->GetMineralPrice() <= currentMinerals) &&
        (bt->GetGasPrice() <= currentGas))
      {
        if (bt->GetStartTime() + 5 < Broodwar->getFrameCount())
        {
          bt->StartBuidling();
        }
      }
    }
  }
}

void BuildingManager::onUnitCreate(BWAPI::Unit unit)
{
  if (!_buildingsInQueue.empty())
  {
    BuildTask* bt = _buildingsInQueue.top();
    if (bt->GetBuildingType() == unit->getType())
    {
      bt->SetProgress(Progress::BUILDING);
      bt->FreeWorker();
      _buildingsInQueue.pop();
      _buildingsInProgress.push_back(bt);
    }
  }
}

void BuildingManager::onUnitComplete(BWAPI::Unit unit)
{
  for (auto bt : _buildingsInProgress)
  {
    if (bt->GetBuildingType() == unit->getType())
    {
      _buildingsInProgress.remove(bt);
      map<BWAPI::UnitType, int>::iterator it = _buildingsMade.find(bt->GetBuildingType());
      if (it != _buildingsMade.end())
      {
        it->second += 1;
      }
      else
      {
        _buildingsMade.insert(pair<BWAPI::UnitType, int>(bt->GetBuildingType(), 1));
      }

      if (unit->getType() == BWAPI::UnitTypes::Protoss_Pylon)
      {
        _pylonInQueue = false;
      }

      delete(bt);
      break;
    }
  }
}