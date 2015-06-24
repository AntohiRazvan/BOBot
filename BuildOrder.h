#pragma once
#include <BWAPI.h>
#include <fstream>
#include "BuildingManager.h"
#include "ProductionManager.h"

class BuildOrder
{

  BuildingManager *_buildingManager;
  ProductionManager *_productionManager;

  void ReadBuildOrder();
public:
  BuildOrder(BuildingManager *bm, ProductionManager *pm);
};
