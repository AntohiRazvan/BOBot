#include "ArmyManager.h"

using namespace std;
using namespace BWAPI;
using namespace BWTA;

ArmyManager::ArmyManager()
{
  _terrainAnalyzer = TerrainAnalyzer::Instance();
  _idlePosition = Position(Broodwar->self()->getStartLocation());
  _attackPosition = Position(Broodwar->self()->getStartLocation());
  for (auto p : Broodwar->getStartLocations())
  {
    if (p != Broodwar->self()->getStartLocation())
    {
      _enemyBases.push_back(Position(p));
    }
  }
}

void ArmyManager::UnderAttackCheck()
{
  if (_visibleEnemyArmy.size() != 0)
  {
    for (auto unit : Broodwar->getUnitsInRadius(_visibleEnemyArmy.front()->getPosition(), 400))
    {
      if ((unit->getType().isBuilding()) && (unit->getPlayer() == Broodwar->self()))
      {
        _attackPosition = _visibleEnemyArmy.front()->getPosition();
        break;
      }
    }
  }
}

void ArmyManager::update()
{
#ifdef DRAWING_ENABLED
  Broodwar->drawCircleMap(_idlePosition, 32, Colors::Yellow);
  Broodwar->drawCircleMap(_attackPosition, 32, Colors::Red);
#endif

  if (_idlePosition == Position(Broodwar->self()->getStartLocation()))
  {
    if (_terrainAnalyzer->Analyzed())
    {
      Chokepoint *chokepoint = _terrainAnalyzer->GetNearestChokepoint(_terrainAnalyzer->GetMyBaseLocation()->getPosition());
      if (chokepoint)
      {
        _idlePosition = chokepoint->getCenter();
      }
    }
  }


  if (_army.size() > 10)
  {
    if (_enemyBuildings.size() != 0)
    {
      _attackPosition = _enemyBuildings.front();
    }
    else
    {
      _attackPosition = *_enemyBases.begin();
    }
  }
  else
  {
    _attackPosition = _idlePosition;
  }
  UnderAttackCheck();


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
    u->attack(_attackPosition);
  }
}

void ArmyManager::Move()
{
  for (auto u : _army)
  {
    u->move(_idlePosition);
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
    auto it = find(_visibleEnemyArmy.begin(), _visibleEnemyArmy.end(), unit);
    if (it != _visibleEnemyArmy.end()) _visibleEnemyArmy.erase(it);
    return;
  }
  if (unit->getType().isResourceDepot())
  {
    auto it = find(_enemyBases.begin(), _enemyBases.end(), unit->getPosition());
    if (it != _enemyBases.end())
    {
      _enemyBases.erase(it);
    }
  }

  auto it = find(_enemyBuildings.begin(), _enemyBuildings.end(), unit->getPosition());
  if (it != _enemyBuildings.end())
  {
    _enemyBuildings.erase(it);
  }
}

void ArmyManager::onEnemyUnitDiscover(Unit unit)
{
  if (unit->getType().isBuilding())
  {
    if (find(_enemyBuildings.begin(), _enemyBuildings.end(), unit->getPosition()) != _enemyBuildings.end())
    {
      return;
    }

    if (unit->getType().isResourceDepot())
    {
      _enemyBases.push_back(unit->getPosition());
    }
    _enemyBuildings.push_back(unit->getPosition());
  }
  else
  {
    _visibleEnemyArmy.push_back(unit);
    _attackPosition = unit->getPosition();
  }
}

void ArmyManager::onEnemyUnitEvade(Unit unit)
{
  if (!unit->getType().isBuilding())
  {
    auto it = find(_visibleEnemyArmy.begin(), _visibleEnemyArmy.end(), unit);
    if (it != _visibleEnemyArmy.end()) _visibleEnemyArmy.erase(it);
    return;
  }
}
