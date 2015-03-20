#include "BuildingManager.h"

using namespace BWAPI;
using namespace Filter;
using namespace std;

BuildingManager::BuildingManager(WorkerManager *wm)
{
  _workerManager = wm;
}

void BuildingManager::AddBuildRequest(UnitType building, Priority priority)
{
  Unit builder = _workerManager->GetWorker();
  BuildTask *buildTask = new BuildTask(builder, building, _workerManager, priority);
  _buildQueue.push(buildTask);
}

void BuildingManager::Update()
{
  SupplyCheck();
  SendBuilders();
  StartBuildingCheck();
  FinishBuildingsCheck();
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
  if (!_buildQueue.empty())
  {
    int currentMinerals = Broodwar->self()->minerals();
    int currentGas = Broodwar->self()->gas();
    
    BuildTask* bt = _buildQueue.top();
    if ((bt->GetMineralPrice() <= currentMinerals) && 
          (bt->GetGasPrice() <= currentGas))
    {
      bt->SendBuilder();
      _buildQueue.pop();
      _buildingsInProgress.push_back(bt);
    }
  }
}

void BuildingManager::StartBuildingCheck()
{
  for (vector<BuildTask*>::iterator it = _buildingsInProgress.begin();
      it != _buildingsInProgress.end(); it++)
  {
    if ((*it)->WorkerArrived() && !(*it)->HasStarted())
    {
      (*it)->StartBuidling();
    }
  }
}

void BuildingManager::FinishBuildingsCheck()
{
  for (vector<BuildTask*>::iterator buildTask = _buildingsInProgress.begin();
    buildTask != _buildingsInProgress.end();)
  {
    if ((*buildTask)->GetStartTime() == 0)
    {
      buildTask++;
      continue;
    }
    if ((*buildTask)->GetStartTime() + (*buildTask)->GetBuildingType().buildTime() < Broodwar->getFrameCount())
    {
      map<UnitType, int>::iterator it = _buildingsMade.find((*buildTask)->GetBuildingType());

      if ((*buildTask)->GetBuildingType() == UnitTypes::Protoss_Pylon) _pylonInQueue = false;
      
      if (it != _buildingsMade.end())
      {
        it->second++;
      }
      else
      {
        _buildingsMade.insert({ (*buildTask)->GetBuildingType(), 1 });
      }
      
      buildTask = _buildingsInProgress.erase(buildTask);
    }
    else
    {
      buildTask++;
    }
  }
}
