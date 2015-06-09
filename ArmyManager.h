#pragma once
#include "Manager.h"
#include "TerrainAnalyzer.h"
#include "BWTA.h"
#include <BWAPI.h>
#include <list>

enum Stance
{
  Idle,
  Attacking
};

class ArmyManager : public Manager
{
  std::list<BWAPI::Unit> _army;
  std::list<BWAPI::Unit> _enemyBuildings;
  std::list<BWAPI::Position> _enemyBases;
  BWAPI::Position _targetPosition;
  Stance _stance = Stance::Idle;
  int _cooldown = 25;
  int _lastOrder = 0;

  void Attack();
  void Move();

public:
  ArmyManager();
  void update();
  void onUnitComplete(BWAPI::Unit unit);
  void onUnitDestroy(BWAPI::Unit unit);
  void onEnemyUnitDestroy(BWAPI::Unit unit);
  void onEnemyUnitDiscover(BWAPI::Unit unit);
};
