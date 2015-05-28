#pragma once
#include <BWAPI.h>
#include "Manager.h"

class ResourceManager : public Manager
{
  int _reservedMinerals;
  int _reservedGas;

public:
  ResourceManager();
  void ReserveMinerals(int mineralsToReserve);
  void ReserveGas(int gasToReserve);
  int GetReservedMinerals();
  int GetReservedGas();
  int GetMinerals();
  int GetGas();
  void FreeMinerals(int mineralsToFree);
  void FreeGas(int gasToFree);
  bool CanAfford(BWAPI::UnitType unitType);
  void onUnitCreate(BWAPI::Unit unit);
};
