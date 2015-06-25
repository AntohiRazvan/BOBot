#pragma once
#include <BWAPI.h>
#include <fstream>
#include "BuildingManager.h"
#include "ProductionManager.h"
#include "Item.h"

class BuildOrder
{
  char* _fileName = "C:\\Users\\Razvan\\Desktop\\buildorder";
  BuildingManager *_buildingManager;
  ProductionManager *_productionManager;

  void ReadBuildOrder();
public:
  BuildOrder(BuildingManager *bm, ProductionManager *pm);
};
