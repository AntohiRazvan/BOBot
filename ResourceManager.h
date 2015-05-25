#pragma once
#include <BWAPI.h>
#include "Manager.h"

class ResourceManager : Manager
{
  ResourceManager *_instance;
  int _reservedMinerals;
  int _reservedGas;

public:
  ResourceManager();
  void ReserveMinerals(int mineralsToReserve);
  void ReserveGas(int gasToReserve);
  int GetReservedMinerals();
  int GetReservedGas();
  void FreeMinerals(int mineralsToFree);
  void FreeGas(int gasToFree);
  void onUnitCreate(BWAPI::Unit unit);
};
