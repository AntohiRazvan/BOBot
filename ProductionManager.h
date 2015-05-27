#pragma once
#include <BWAPI.h>
#include <vector>
#include "Manager.h"
#include "ResourceManager.h"
#include "BuildingManager.h"
#include "TerrainAnalyzer.h"

class ProductionManager : public Manager
{
  ResourceManager *_resourceManager;
  BuildingManager *_buildingManager;
  std::vector<BWAPI::Unit> _productionBuildings;
  BWAPI::Position _rallyPoint;

public:
  ProductionManager(ResourceManager *rm, BuildingManager *bm);
  void registerProductionBuilding(BWAPI::Unit unit);
  void setRallyPoint(BWAPI::Position position);

  void onUnitCreate(BWAPI::Unit unit);
  void onUnitDestroy(BWAPI::Unit unit);
  void update();
};

