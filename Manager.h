#pragma once
#include <BWAPI.h>

class Manager
{
public:
  Manager();
  virtual void update();
  virtual void onUnitComplete(BWAPI::Unit unit);
  virtual void onUnitCreate(BWAPI::Unit unit);
  virtual void onUnitDestroy(BWAPI::Unit unit);
};

