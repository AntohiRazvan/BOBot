#include "Licenta.h"

using namespace BWAPI;
using namespace Filter;

void BWBOB::onStart()
{
#ifdef GUI_DISABLED
  Broodwar->setGUI(false);
#endif

  TerrainAnalyzer::Instance();

  Broodwar << "The map is " << Broodwar->mapName() << "!" << std::endl;
  Broodwar->enableFlag(Flag::UserInput);
  Broodwar->setCommandOptimizationLevel(2);

  if (Broodwar->isReplay())
  {
    Broodwar << "The following players are in this replay:" << std::endl;
    Playerset players = Broodwar->getPlayers();
    for (auto p : players)
    {
      if (!p->isObserver())
        Broodwar << p->getName() << ", playing as " << p->getRace() << std::endl;
    }
  }
  else
  {
    if (Broodwar->enemy())
      Broodwar << "The matchup is " << Broodwar->self()->getRace() << " vs " << Broodwar->enemy()->getRace() << std::endl;
  }
}

void BWBOB::onFrame()
{
  TerrainAnalyzer::Instance()->DrawTerrainDataWhenAble();
  if (!_managersInitialised)
  {
    _armyManager = new ArmyManager();
    _resourceManager = new ResourceManager();
    _productionManager = new ProductionManager(_resourceManager);
    _workerManager = new WorkerManager(_productionManager);
    _scoutManager = new ScoutManager(_workerManager);
    _buildingManager = new BuildingManager(_workerManager, _resourceManager);
    _buildOrder = new BuildOrder(_buildingManager, _productionManager, _armyManager);

    _managers.push_back(_resourceManager);
    _managers.push_back(_productionManager);
    _managers.push_back(_workerManager);
    _managers.push_back(_buildingManager);
    _managers.push_back(_armyManager);
    _managers.push_back(_scoutManager);
    _managers.push_back(_buildOrder);

#ifdef LOGGING_ENABLED
    _log = new Logger();
    _managers.push_back(_log);
#endif

    _managersInitialised = true;
  }

  Broodwar->drawTextScreen(200, 0, "FPS: %d", Broodwar->getFPS());
  Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS());

  if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
    return;

  if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
    return;

  for (auto manager : _managers)
  {
    manager->update();
  } 
 }

void BWBOB::onSendText(std::string text)
{
}

void BWBOB::onReceiveText(Player player, std::string text)
{
}

void BWBOB::onPlayerLeft(Player player)
{
}

void BWBOB::onNukeDetect(Position target)
{
}

void BWBOB::onUnitDiscover(Unit unit)
{
  if (!(unit->getPlayer() == Broodwar->self()) &&
      !(unit->getPlayer() == Broodwar->neutral()))
  {
    for (auto manager : _managers)
    {
      manager->onEnemyUnitDiscover(unit);
    }
  }
}

void BWBOB::onUnitEvade(Unit unit)
{
  if (!(unit->getPlayer() == Broodwar->self()) &&
      !(unit->getPlayer() == Broodwar->neutral()))
  {
    for (auto manager : _managers)
    {
      manager->onEnemyUnitEvade(unit);
    }
  }
}

void BWBOB::onUnitShow(Unit unit)
{
}

void BWBOB::onUnitHide(Unit unit)
{
}

void BWBOB::onUnitCreate(Unit unit)
{
  if (unit->getPlayer() == Broodwar->self() || 
      unit->getPlayer() == Broodwar->neutral())
  {
    for (auto manager : _managers)
    {
      manager->onUnitCreate(unit);
    }
  }
}

void BWBOB::onUnitDestroy(Unit unit)
{
  if (unit->getPlayer() == Broodwar->self())
  {
    for (auto manager : _managers)
    {
      manager->onUnitDestroy(unit);
    }
  }
  else
  {
    for (auto manager : _managers)
    {
      manager->onEnemyUnitDestroy(unit);
    }
  }
}

void BWBOB::onUnitMorph(Unit unit)
{
  if (unit->getPlayer() == Broodwar->self() || 
      unit->getPlayer() == Broodwar->neutral())
  {
    for (auto manager : _managers)
    {
      manager->onUnitMorph(unit);
    }
  }
}

void BWBOB::onUnitRenegade(Unit unit)
{
}

void BWBOB::onSaveGame(std::string gameName)
{
  Broodwar << "The game was saved to \"" << gameName << "\"" << std::endl;
}

void BWBOB::onUnitComplete(Unit unit)
{
  if (unit->getPlayer() == Broodwar->self())
  {
    for (auto manager : _managers)
    {
      manager->onUnitComplete(unit);
    }
  }
}

void BWBOB::onEnd(bool isWinner)
{
  for (auto manager : _managers)
  {
    manager->onEnd(isWinner);
  }
}
