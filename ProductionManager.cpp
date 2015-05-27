#include "ProductionManager.h"
#include <BWTA.h>
using namespace BWAPI;

ProductionManager::ProductionManager(ResourceManager *rm, BuildingManager *bm)
{
  _resourceManager = rm;
  _buildingManager = bm;
}


void ProductionManager::registerProductionBuilding(BWAPI::Unit unit)
{
  _productionBuildings.push_back(unit);
}

void ProductionManager::setRallyPoint(BWAPI::Position position)
{
  _rallyPoint = position;
}

void ProductionManager::onUnitCreate(BWAPI::Unit unit)
{

}

void ProductionManager::onUnitDestroy(BWAPI::Unit unit)
{

}

void ProductionManager::update()
{
  if (!_rallyPoint)
  {
    TerrainAnalyzer *ta = TerrainAnalyzer::Instance();
    _rallyPoint = ta->GetNearestChokepoint(ta->GetMyBaseLocation())->getCenter();
  }
}


