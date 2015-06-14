#include "ArmyManager.h"

using namespace std;
using namespace BWAPI;
using namespace BWTA;

ArmyManager::ArmyManager()
{
  _targetPosition = Position(Broodwar->self()->getStartLocation());
  for (auto p : Broodwar->getStartLocations())
  {
    if (p != Broodwar->self()->getStartLocation())
    {
      _enemyBases.push_back(Position(p));
    }
  }
}

void ArmyManager::update()
{
#ifdef DRAWING_ENABLED
  Broodwar->drawCircleMap(_targetPosition, 32, Colors::Red);
#endif
  TerrainAnalyzer *ta = TerrainAnalyzer::Instance();
  if (_targetPosition == Position(Broodwar->self()->getStartLocation()))
  {
    Chokepoint *chokepoint = ta->GetNearestChokepoint(ta->GetMyBaseLocation());
    if (chokepoint)
    {
      _targetPosition = chokepoint->getCenter();
    }
  }
  if (_army.size() > 3)
  {
    _targetPosition = *_enemyBases.begin();
  }

  if (_lastOrder + _cooldown < Broodwar->getFrameCount())
  {
    _lastOrder = Broodwar->getFrameCount();
    Attack();
  }
}

void ArmyManager::Attack()
{
  for (auto u : _army)
  {
    u->attack(_targetPosition);
  }
}

void ArmyManager::Move()
{
  for (auto u : _army)
  {
    u->move(_targetPosition);
  }
}

void ArmyManager::onUnitComplete(Unit unit)
{
  if ((unit->getType().isBuilding()) || (unit->getType() == UnitTypes::Protoss_Probe))
  {
    return;
  }
  Attack();
  _army.push_back(unit);
}

void ArmyManager::onUnitDestroy(Unit unit)
{
  if ((unit->getType().isBuilding()) || (unit->getType() == UnitTypes::Protoss_Probe))
  {
    return;
  }
 
  list<Unit>::iterator it = find(_army.begin(), _army.end(), unit);
  if (it != _army.end())
  {
    _army.erase(it);
  }
}

void ArmyManager::onEnemyUnitDestroy(Unit unit)
{
  if (!unit->getType().isBuilding())
  {
    return;
  }
  if (unit->getType().isResourceDepot())
  {
    list<Position>::iterator it = find(_enemyBases.begin(), _enemyBases.end(), unit->getPosition());
    if (it != _enemyBases.end())
    {
      _enemyBases.erase(it);
    }
  }

  list<Unit>::iterator it = find(_enemyBuildings.begin(), _enemyBuildings.end(), unit);
  if (it != _enemyBuildings.end())
  {
    _enemyBuildings.erase(it);
  }
}

void ArmyManager::onEnemyUnitDiscover(Unit unit)
{
  if (!unit->getType().isBuilding())
  {
    if (find(_enemyBuildings.begin(), _enemyBuildings.end(), unit) == _enemyBuildings.end())
    {
      return;
    }

    if (unit->getType().isResourceDepot())
    {
      _enemyBases.push_back(unit->getPosition());
    }
    _enemyBuildings.push_back(unit);
  }
  else
  {
    _targetPosition = unit->getPosition();
  }
}
