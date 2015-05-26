#pragma once
#include <BWAPI.h>
#include <vector>
#include "Manager.h"
#include "ResourceManager.h"
#include "BuildingManager.h"

class ProductionManager : public Manager
{
  ResourceManager *_resourceManager;
  BuildingManager *_buildingManager;
  std::vector<BWAPI::Unit> _productionBuildings;
  BWAPI::TilePosition _rallyPoint;

public:
  ProductionManager(ResourceManager *rm, BuildingManager *bm);
  void registerProductionBuilding(BWAPI::Unit);
  void setRallyPoint(BWAPI::TilePosition);

  void onUnitCreate(BWAPI::Unit unit);
  void onUnitDestroy(BWAPI::Unit unit);
  void update();
};

