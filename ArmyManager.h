#pragma once
#include "Manager.h"
#include "TerrainAnalyzer.h"
#include "BWTA.h"
#include <BWAPI.h>
#include <list>

enum Stance
{
  Idle,
  Attacking,
  Searching
};

class ArmyManager : public Manager
{
  int _maxArmySize = 0;
  bool _isAttacking = false;
  bool _isDefending = false;
  TerrainAnalyzer *_terrainAnalyzer;
  std::list<BWAPI::Unit> _army;
  std::list<BWAPI::Position> _enemyBuildings;
  std::list<BWAPI::Position> _enemyBases;
  std::list<BWAPI::Unit> _visibleEnemyArmy;
  BWAPI::Position _attackPosition;
  BWAPI::Position _idlePosition;
  Stance _stance = Stance::Idle;
  int _cooldown = 30;
  int _lastOrder = 0;

  int _lastBuildingPop = 0;
  int _enemyBuildingPopRate = 2000;

  void Move();
  void UnderAttackCheck();
public:
  ArmyManager();
  void Attack();

  void update();
  void onUnitComplete(BWAPI::Unit unit);
  void onUnitDestroy(BWAPI::Unit unit);
  void onEnemyUnitDestroy(BWAPI::Unit unit);
  void onEnemyUnitDiscover(BWAPI::Unit unit); 
  void onEnemyUnitEvade(BWAPI::Unit unit);
};
