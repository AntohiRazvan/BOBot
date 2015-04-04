#pragma once
#include <BWAPI.h>

class Manager
{
public:
  Manager();
  void Update();
  void onUnitComplete(BWAPI::Unit unit);
  void onUnitCreate(BWAPI::Unit unit);
};

