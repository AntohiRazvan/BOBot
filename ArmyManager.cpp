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
    for (auto unit : Broodwar->getUnitsInRadius(_visibleEnemyArmy.front()->getPosition(), 1000))
    {
      if ((unit->getPlayer() == Broodwar->self()) && (!Filter::IsWorker(unit)))
      {
        _attackPosition = _visibleEnemyArmy.front()->getPosition();
        _isDefending = true;
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
  /*
  if ((_isAttacking && _lastBuildingPop + _enemyBuildingPopRate < Broodwar->getFrameCount()) &&
       _enemyBuildings.size() > 1)
  {
    _enemyBuildings.pop_back();
  }*/

  if (_enemyBuildings.size() != 0)
  {
    _attackPosition = _enemyBuildings.back();
  }
  else
  {
    _attackPosition = *_enemyBases.begin();
  }

  UnderAttackCheck();

  if (_lastOrder + _cooldown < Broodwar->getFrameCount())
  {
    _lastOrder = Broodwar->getFrameCount();
    Move();
    if (_isDefending)
    {
      for (auto u : _army)
      {
        u->attack(_attackPosition);
      }
    }
    if (_isAttacking)
    {
  /*    if (_maxArmySize > 25 && _army.size() > 6)
      {
        for (auto u : _army)
        {
          u->attack(_attackPosition);
        }
      }
      else if (_maxArmySize < 25)
      {
        for (auto u : _army)
        {
          u->attack(_attackPosition);
        }
      }*/
      for (auto u : _army)
      {
        u->attack(_attackPosition);
      }
    }
  }
  _isDefending = false;
}

void ArmyManager::Attack()
{
  _isAttacking = !_isAttacking;
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
  Move();
  _maxArmySize++;
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
  
  auto it = find(_enemyBuildings.begin(), _enemyBuildings.end(), unit->getPosition());
  if (it != _enemyBuildings.end())
  {
    _enemyBuildings.erase(it);
  }
  
  if (unit->getType().isResourceDepot())
  {
    auto it = find(_enemyBases.begin(), _enemyBases.end(), unit->getPosition());
    if (it != _enemyBases.end())
    {
      _enemyBases.erase(it);
    }
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
      _enemyBases.push_front(unit->getPosition());
    }
    _enemyBuildings.push_front(unit->getPosition());
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
