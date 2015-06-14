#include "ScoutManager.h"

using namespace std;
using namespace BWAPI;
using namespace BWTA;

ScoutManager::ScoutManager(WorkerManager *wm)
{
  _scout = nullptr;
  _workerManager = wm;
  _terrainAnalyzer = TerrainAnalyzer::Instance();
}

void ScoutManager::InitialScout()
{
  for (auto startLocation : Broodwar->getStartLocations())
  {
    auto startPosition = startLocation;
    if (startPosition != Broodwar->self()->getStartLocation())
    {
      _locationsToScout.push(Position(startPosition));
    }
  }
}

void ScoutManager::Scout()
{
  if (!(_lastScoutTime + _scoutFrequency < Broodwar->getFrameCount()))
  {
    return;
  }

  for (auto baseLocation : _terrainAnalyzer->GetBaseLocations())
  {
    auto it = find(_enemyBaseLocations.begin(), _enemyBaseLocations.end(), baseLocation->getPosition());
    if ((!BaseIsMine(baseLocation)) && (it == _enemyBaseLocations.end()))
    {
      _locationsToScout.push(baseLocation->getPosition());
    }
  }
}

bool ScoutManager::BaseIsMine(BaseLocation* baseLocation)
{
  auto units = Broodwar->getUnitsInRadius(baseLocation->getPosition(), 500);
  for (auto unit : units)
  {
    if (unit->getType().isBuilding() && unit->getPlayer() == Broodwar->self())
    {
      return true;
    }
  }
  return false;
}

void ScoutManager::update()
{
  if (Broodwar->getFrameCount() == _initialScoutTime)
  {
    InitialScout();
  }

  if (_locationsToScout.size() != 0)
  {
    if (!_scout)
    {
      _scout = _workerManager->GetWorker();
    }
    if (_scout->getPosition().getApproxDistance(_locationsToScout.front()) < 15)  
    {      
      _locationsToScout.pop();
    }
    else { 
      _scout->move(_locationsToScout.front());
    }
  }
  else
  {
    if (_scout)
    {
      _lastScoutTime = Broodwar->getFrameCount();
      _workerManager->AddWorker(_scout);
      _scout = nullptr;
    }
  }
}

void ScoutManager::onUnitDestroy(Unit unit)
{
  if (unit == _scout)
  {
    _scout = nullptr;
    if (_locationsToScout.size() != 0)
    {
      _locationsToScout.pop();
    }
  }
}

void ScoutManager::onEnemyUnitDiscover(Unit unit)
{
  auto it = find(_enemyBuildings.begin(), _enemyBuildings.end(), unit);
  if (it != _enemyBuildings.end()) _enemyBuildings.push_back(unit);
}


vector<Unit> ScoutManager::GetEnemyBuildings()
{
  return _enemyBuildings;
}

vector<Position> ScoutManager::GetEnemyBaseLocations()
{
  return _enemyBaseLocations;
}
