#include "ResourceManager.h"

using namespace BWAPI;

ResourceManager::ResourceManager()
{
  _reservedMinerals = 0;
  _reservedGas = 0;
}

void ResourceManager::ReserveMinerals(int mineralsToReserve)
{
  _reservedMinerals += mineralsToReserve;
}

void ResourceManager::ReserveGas(int gasToReserve)
{
  _reservedGas += gasToReserve;
}

int ResourceManager::GetReservedMinerals()
{
  return _reservedMinerals;
}

int ResourceManager::GetReservedGas()
{
  return _reservedGas;
}

int ResourceManager::GetMinerals()
{
  return Broodwar->self()->minerals() - _reservedMinerals;
}

int ResourceManager::GetGas()
{
  return Broodwar->self()->gas() - _reservedGas;
}

void ResourceManager::FreeMinerals(int mineralsToFree)
{
  _reservedMinerals -= mineralsToFree;
}

void ResourceManager::FreeGas(int gasToFree)
{
  _reservedMinerals -= gasToFree;
}

void ResourceManager::onUnitCreate(Unit unit)
{
  if (Broodwar->getFrameCount() > 1 && unit->getType().isBuilding())
  {
    _reservedMinerals -= unit->getType().mineralPrice();
    _reservedGas -= unit->getType().gasPrice();
  }
}

void ResourceManager::onUnitMorph(Unit unit)
{
  if (Broodwar->getFrameCount() > 1 && unit->getType().isBuilding())
  {
    _reservedMinerals -= unit->getType().mineralPrice();
    _reservedGas -= unit->getType().gasPrice();
  }
}

bool ResourceManager::CanAfford(UnitType unitType)
{
  bool canAfford = true;
  int mineralPrice = unitType.mineralPrice();
  int gasPrice = unitType.gasPrice();
  if ((mineralPrice != 0) && (Broodwar->self()->minerals() - mineralPrice - _reservedMinerals < 0))
    canAfford = false;
  if ((gasPrice != 0) && (Broodwar->self()->gas() - gasPrice - _reservedGas < 0))
    canAfford = false;
  return canAfford;
}