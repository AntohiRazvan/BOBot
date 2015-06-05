#include "Licenta.h"

using namespace BWAPI;
using namespace Filter;

void ExampleAIModule::onStart()
{
  Broodwar->setGUI(false);
  TerrainAnalyzer::Instance();
  _resourceManager = new ResourceManager();
  _workerManager = new WorkerManager();
  _buildingManager = new BuildingManager(_workerManager, _resourceManager);
  _productionManager = new ProductionManager(_resourceManager, _buildingManager);

  _managers.push_back(_resourceManager);
  _managers.push_back(_productionManager);
  _managers.push_back(_buildingManager);
  _managers.push_back(_workerManager);
  
#ifdef LOGGING_ENABLED
  _log = new Logger();
  _managers.push_back(_log);
#endif

  // Print the map name.
  // BWAPI returns std::string when retrieving a string, don't forget to add .c_str() when printing!
  Broodwar << "The map is " << Broodwar->mapName() << "!" << std::endl;
  Broodwar->enableFlag(Flag::UserInput);

  // Uncomment the following line and the bot will know about everything through the fog of war (cheat).
  //Broodwar->enableFlag(Flag::CompleteMapInformation);

  // Set the command optimization level so that common commands can be grouped
  // and reduce the bot's APM (Actions Per Minute).
  Broodwar->setCommandOptimizationLevel(2);

  // Check if this is a replay
  if ( Broodwar->isReplay() )
  {

    // Announce the players in the replay
    Broodwar << "The following players are in this replay:" << std::endl;
 
    // Iterate all the players in the game using a std:: iterator
    Playerset players = Broodwar->getPlayers();
    for(auto p : players)
    {
      // Only print the player if they are not an observer
      if ( !p->isObserver() )
        Broodwar << p->getName() << ", playing as " << p->getRace() << std::endl;
    }

  }
  else // if this is not a replay
  {
    // Retrieve you and your enemy's races. enemy() will just return the first enemy.
    // If you wish to deal with multiple enemies then you must use enemies().
    if ( Broodwar->enemy() ) // First make sure there is an enemy
      Broodwar << "The matchup is " << Broodwar->self()->getRace() << " vs " << Broodwar->enemy()->getRace() << std::endl;
  }

}

void ExampleAIModule::onFrame()
{
  TerrainAnalyzer::Instance()->DrawTerrainDataWhenAble();
  static bool a = true;
  if (a)
  {
    _buildingManager->AddBuildRequest(BWAPI::UnitTypes::Protoss_Gateway);
    _buildingManager->AddBuildRequest(BWAPI::UnitTypes::Protoss_Gateway);
    a = false;
  }

  // Called once every game frame

  // Display the game frame rate as text in the upper left area of the screen
  Broodwar->drawTextScreen(200, 0, "FPS: %d", Broodwar->getFPS());
  Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS());

  // Return if the game is a replay or is paused
  if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
    return;

  // Prevent spamming by only running our onFrame once every number of latency frames.
  // Latency frames are the number of frames before commands are processed.
  if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
    return;

  for (auto manager : _managers)
  {
    manager->update();
  } 
 }

void ExampleAIModule::onSendText(std::string text)
{
}

void ExampleAIModule::onReceiveText(BWAPI::Player player, std::string text)
{
}

void ExampleAIModule::onPlayerLeft(BWAPI::Player player)
{
}

void ExampleAIModule::onNukeDetect(BWAPI::Position target)
{
}

void ExampleAIModule::onUnitDiscover(BWAPI::Unit unit)
{
  if (!(unit->getPlayer() == BWAPI::Broodwar->self()))
  {
    for (auto manager : _managers)
    {
      manager->onEnemyUnitDiscover(unit);
    }
  }
}

void ExampleAIModule::onUnitEvade(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitShow(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitHide(BWAPI::Unit unit)
{
}
#include <vector>
void ExampleAIModule::onUnitCreate(BWAPI::Unit unit)
{
  static TilePosition pos;
  if (pos)
  {
    for (auto p : Broodwar->getStartLocations())
    {
      if (p != Broodwar->self()->getStartLocation())
      {
        pos = TilePosition(p);
      }
    }
  }

  static std::vector<BWAPI::Unit> units;
  if (unit->getType() == UnitTypes::Protoss_Zealot)
  {
    units.push_back(unit);
  }
  if (units.size() > 4)
  {
    for (auto u : units)
    {
      u->attack(Position(pos));
    }
  }
  if (unit->getPlayer() == BWAPI::Broodwar->self())
  {
    for (auto manager : _managers)
    {
      manager->onUnitCreate(unit);
    }
  }
}

void ExampleAIModule::onUnitDestroy(BWAPI::Unit unit)
{
  if (unit->getPlayer() == BWAPI::Broodwar->self())
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

void ExampleAIModule::onUnitMorph(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitRenegade(BWAPI::Unit unit)
{
}

void ExampleAIModule::onSaveGame(std::string gameName)
{
  Broodwar << "The game was saved to \"" << gameName << "\"" << std::endl;
}

void ExampleAIModule::onUnitComplete(BWAPI::Unit unit)
{
  if (unit->getPlayer() == BWAPI::Broodwar->self())
  {
    for (auto manager : _managers)
    {
      manager->onUnitComplete(unit);
    }
  }
}
#include <iostream>
void ExampleAIModule::onEnd(bool isWinner)
{
  for (auto manager : _managers)
  {
    manager->onEnd(isWinner);
  }
}
