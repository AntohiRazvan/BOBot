#pragma once
#include <BWAPI.h>
#include "BuildingManager.h"
#include "ProductionManager.h"

class BuildOrder
{
  BuildingManager *_buildingManager;
  ProductionManager *_productionManager;

public:
  BuildOrder(BuildingManager *bm, ProductionManager *pm);
};
