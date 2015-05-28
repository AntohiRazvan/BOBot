#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <vector>
#include <algorithm>
#include "Manager.h"
#include "ResourceManager.h"
#include "BuildingManager.h"
#include "TerrainAnalyzer.h"
#include "UnitProducer.h"

class ProductionManager : public Manager
{
  ResourceManager *_resourceManager;
  BuildingManager *_buildingManager;
  UnitProducer *_gatewayProduction;
  UnitProducer *_stargateProduction;
  UnitProducer *_roboProduction;
  UnitProducer *_nexusProduction;


public:
  ProductionManager(ResourceManager *rm, BuildingManager *bm);
  void registerProductionBuilding(BWAPI::Unit unit);
  void Train(BWAPI::UnitType unitType);
  void Train(BWAPI::UnitType unitType, int count);

  void onUnitComplete(BWAPI::Unit unit);
  void onUnitDestroy(BWAPI::Unit unit);
  void update();
};
