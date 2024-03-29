#pragma once
#include <list>
#include <BWAPI.h>
#include "..\Manager.h"
#include "..\BuildingManager.h"
#include "..\WorkerManager.h"
#include "..\ResourceManager.h"
#include "..\TerrainAnalyzer.h"
#include "..\ProductionManager.h"
#include "..\ArmyManager.h"
#include "..\ScoutManager.h"
#include "..\Logger.h"
#include "..\BuildOrder.h"

#define LOGGING_ENABLED
//#define GUI_DISABLED

class BWBOB : public BWAPI::AIModule
{
  bool _managersInitialised = false;
  WorkerManager *_workerManager;
  BuildingManager *_buildingManager;
  ResourceManager *_resourceManager;
  ProductionManager *_productionManager;
  ArmyManager *_armyManager;
  BuildOrder *_buildOrder;
  Logger *_log;
  ScoutManager *_scoutManager;
  std::list<Manager*> _managers;


public:
  // Virtual functions for callbacks, leave these as they are.
  virtual void onStart();
  virtual void onEnd(bool isWinner);
  virtual void onFrame();
  virtual void onSendText(std::string text);
  virtual void onReceiveText(BWAPI::Player player, std::string text);
  virtual void onPlayerLeft(BWAPI::Player player);
  virtual void onNukeDetect(BWAPI::Position target);
  virtual void onUnitDiscover(BWAPI::Unit unit);
  virtual void onUnitEvade(BWAPI::Unit unit);
  virtual void onUnitShow(BWAPI::Unit unit);
  virtual void onUnitHide(BWAPI::Unit unit);
  virtual void onUnitCreate(BWAPI::Unit unit);
  virtual void onUnitDestroy(BWAPI::Unit unit);
  virtual void onUnitMorph(BWAPI::Unit unit);
  virtual void onUnitRenegade(BWAPI::Unit unit);
  virtual void onSaveGame(std::string gameName);
  virtual void onUnitComplete(BWAPI::Unit unit);
  // Everything below this line is safe to modify.
};
