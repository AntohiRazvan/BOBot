#include "ProductionManager.h"

using namespace std;
using namespace BWAPI;
using namespace BWTA;

ProductionManager::ProductionManager(ResourceManager *rm, BuildingManager *bm)
{
  _resourceManager = rm;
  _buildingManager = bm;
  _gatewayProduction = new UnitProducer(rm);
  _stargateProduction = new UnitProducer(rm);
  _roboProduction = new UnitProducer(rm);
  _nexusProduction = new UnitProducer(rm);
  _nexusProduction->SetFocus(UnitTypes::Protoss_Probe);
  _gatewayProduction->SetFocus(UnitTypes::Protoss_Zealot);


}


void ProductionManager::registerProductionBuilding(BWAPI::Unit unit)
{
  if (unit->getType() == UnitTypes::Protoss_Gateway)
  {
    _gatewayProduction->AddProducer(unit);
  }
  else  if (unit->getType() == UnitTypes::Protoss_Robotics_Facility)
  {
    _roboProduction->AddProducer(unit);
  }
  else  if (unit->getType() == UnitTypes::Protoss_Stargate)
  {
    _stargateProduction->AddProducer(unit);
  }
  else if (unit->getType() == UnitTypes::Protoss_Nexus)
  {
    _nexusProduction->AddProducer(unit);
  }
}

void ProductionManager::onUnitComplete(BWAPI::Unit unit)
{
  registerProductionBuilding(unit);
}

void ProductionManager::onUnitDestroy(BWAPI::Unit unit)
{
  UnitType type = unit->getType();
  if (type == UnitTypes::Protoss_Gateway)
  {
    _gatewayProduction->RemoveProducer(unit);
  }
  else if (type == UnitTypes::Protoss_Robotics_Facility)
  {
    _roboProduction->RemoveProducer(unit);
  }
  else if (type == UnitTypes::Protoss_Stargate)
  {
    _stargateProduction->RemoveProducer(unit);
  }
  else if (type == UnitTypes::Protoss_Nexus)
  {
    _nexusProduction->RemoveProducer(unit);
  }
}

void ProductionManager::Train(BWAPI::UnitType unitType)
{
  UnitType productionRequired = unitType.whatBuilds().first;
  if (productionRequired == UnitTypes::Protoss_Gateway)
  {
    _gatewayProduction->SetFocus(unitType);
  }
  else if (productionRequired == UnitTypes::Protoss_Robotics_Facility)
  {
    _roboProduction->SetFocus(unitType);
  }
  else if (productionRequired == UnitTypes::Protoss_Stargate)
  {
    _stargateProduction->SetFocus(unitType);
  }
  else if (productionRequired == UnitTypes::Protoss_Nexus)
  {
    _nexusProduction->SetFocus(unitType);
  }
}

void ProductionManager::Train(BWAPI::UnitType unitType, int count)
{
  UnitType productionRequired = unitType.whatBuilds().first;
  if (productionRequired == UnitTypes::Protoss_Gateway)
  {
    _gatewayProduction->SetFocus(unitType, count);
  }
  else if (productionRequired == UnitTypes::Protoss_Robotics_Facility)
  {
    _roboProduction->SetFocus(unitType, count);
  }
  else if (productionRequired == UnitTypes::Protoss_Stargate)
  {
    _stargateProduction->SetFocus(unitType, count);
  }
  else if (productionRequired == UnitTypes::Protoss_Nexus)
  {
    _nexusProduction->SetFocus(unitType, count);
  }
}

void ProductionManager::update()
{
  _nexusProduction->Produce();
  _gatewayProduction->Produce();
  _roboProduction->Produce();
  _stargateProduction->Produce();
}
