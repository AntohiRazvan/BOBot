#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <vector>
#include <algorithm>
#include "Manager.h"
#include "ResourceManager.h"
#include "TerrainAnalyzer.h"
#include "UnitProducer.h"

class ProductionManager : public Manager
{
  ResourceManager *_resourceManager;
  UnitProducer *_gatewayProduction;
  UnitProducer *_stargateProduction;
  UnitProducer *_roboProduction;
  UnitProducer *_nexusProduction;


public:
  ProductionManager(ResourceManager *rm);
  void registerProductionBuilding(BWAPI::Unit unit);
  void Train(BWAPI::UnitType unitType);
  void Train(BWAPI::UnitType unitType, int count);
  void HaltGatewayProduction();
  void HaltStargateProduction();
  void HaltRoboProduction();
  void HaltNexusProduction();
  void ResumeGatewayProduction();
  void ResumeStargateProduction();
  void ResumeRoboProduction();
  void ResumeNexusProduction();


  void onUnitComplete(BWAPI::Unit unit);
  void onUnitDestroy(BWAPI::Unit unit);
  void update();
};
