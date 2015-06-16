#include "BuildOrder.h"

using namespace BWAPI;

BuildOrder::BuildOrder(BuildingManager *bm, ProductionManager *pm)
{
  _buildingManager = bm;
  _productionManager = pm;

  pm->Train(UnitTypes::Protoss_Dragoon);
  pm->Train(UnitTypes::Protoss_Reaver);

  bm->AddBuildRequest(UnitTypes::Protoss_Gateway);
  bm->AddBuildRequest(UnitTypes::Protoss_Assimilator);
  bm->AddBuildRequest(UnitTypes::Protoss_Nexus);
  bm->AddBuildRequest(UnitTypes::Protoss_Cybernetics_Core);
  bm->AddBuildRequest(UnitTypes::Protoss_Robotics_Facility);
  bm->AddBuildRequest(UnitTypes::Protoss_Robotics_Support_Bay);
  bm->AddBuildRequest(UnitTypes::Protoss_Gateway);
  bm->AddBuildRequest(UnitTypes::Protoss_Gateway);
}
