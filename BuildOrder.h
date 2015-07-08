#pragma once
#include <BWAPI.h>
#include <fstream>
#include <queue>
#include "Manager.h"
#include "ArmyManager.h"
#include "BuildingManager.h"
#include "ProductionManager.h"
#include "Item.h"

class BuildOrder : public Manager
{
  char* _fileName = "C:\\Program Files (x86)\\StarCraft\\bwapi-data\\Build Orders\\buildorder.bo";
  BuildingManager *_buildingManager;
  ProductionManager *_productionManager;
  ArmyManager *_armyManager;  
  std::queue<Item*> _buildOrder;
  std::map<BWAPI::UnitType, int> _unitsMade;
  bool _itemStarted = false;

  void ReadBuildOrder();
public:
  BuildOrder(BuildingManager *bm, ProductionManager *pm, ArmyManager *am);
  void update();
  void onUnitComplete(BWAPI::Unit unit);
  void onUnitMorph(BWAPI::Unit unit);
  void onUnitCreate(BWAPI::Unit unit);
};
