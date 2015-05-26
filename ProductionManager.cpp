#include "ProductionManager.h"
#include <BWTA.h>
using namespace BWAPI;

ProductionManager::ProductionManager(ResourceManager *rm, BuildingManager *bm)
{
  _resourceManager = rm;
  _buildingManager = bm;
}


void ProductionManager::registerProductionBuilding(BWAPI::Unit)
{

}

void ProductionManager::setRallyPoint(BWAPI::TilePosition)
{

}

void ProductionManager::onUnitCreate(BWAPI::Unit unit)
{

}

void ProductionManager::onUnitDestroy(BWAPI::Unit unit)
{

}

void ProductionManager::update()
{

}
