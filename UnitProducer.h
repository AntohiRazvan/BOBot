#pragma once
#include <BWAPI.h>
#include <vector>
#include "ResourceManager.h"

class UnitProducer
{
  ResourceManager *_resourceManager;
  std::vector<BWAPI::Unit> _producers;
  BWAPI::UnitType _mainFocus;
  BWAPI::UnitType _tempFocus;
  int _tempFocusCount;
  bool _halted;

public:
  UnitProducer(ResourceManager *rm);
  void Produce();
  void AddProducer(BWAPI::Unit unit);
  void RemoveProducer(BWAPI::Unit unit);
  void SetFocus(BWAPI::UnitType unit);
  void SetFocus(BWAPI::UnitType unit, int count);
  void Halt();
  void Resume();
};

