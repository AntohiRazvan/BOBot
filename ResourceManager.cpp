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
  if (Filter::IsBuilding(unit))
  {
    _reservedMinerals -= unit->getType().mineralPrice();
    _reservedGas -= unit->getType().gasPrice();
  }
}
