#include "Licenta.h"
#include <iostream>


using namespace BWAPI;
using namespace Filter;

void ExampleAIModule::onStart()
{
  _workerManager = new WorkerManager();
  _buildingManager = new BuildingManager(_workerManager);

  _managers.push_back(_workerManager);
  _managers.push_back(_buildingManager);

  // Hello World!
  Broodwar->sendText("Hello world!");

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

void ExampleAIModule::onEnd(bool isWinner)
{
  // Called when the game ends
  if ( isWinner )
  {
    // Log your win here!
  }
}

void ExampleAIModule::onFrame()
{
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
    manager->Update();
  }

  // Iterate through all the units that we own
  for (auto &u : Broodwar->self()->getUnits())
  {
    // Ignore the unit if it no longer exists
    // Make sure to include this block when handling any Unit pointer!
    if (!u->exists())
      continue;

    // Ignore the unit if it has one of the following status ailments
    if (u->isLockedDown() || u->isMaelstrommed() || u->isStasised())
      continue;

    // Ignore the unit if it is in one of the following states
    if (u->isLoaded() || !u->isPowered() || u->isStuck())
      continue;

    // Ignore the unit if it is incomplete or busy constructing
    if (!u->isCompleted() || u->isConstructing())
      continue;


    else if (u->getType().isResourceDepot()) // A resource depot is a Command Center, Nexus, or Hatchery
    {

      // Order the depot to construct more workers! But only when it is idle.
      if (u->isIdle() && !u->train(u->getType().getRace().getWorker()))
      {
        // If that fails, draw the error at the location so that you can visibly see what went wrong!
        // However, drawing the error once will only appear for a single frame
        // so create an event that keeps it on the screen for some frames
        Position pos = u->getPosition();
        Error lastErr = Broodwar->getLastError();
        Broodwar->registerEvent([pos, lastErr](Game*){ Broodwar->drawTextMap(pos, "%c%s", Text::White, lastErr.c_str()); },   // action
          nullptr,    // condition
          Broodwar->getLatencyFrames());  // frames to run

      } // closure: failed to train idle unit

    }
    if (u->getType() == BWAPI::UnitTypes::Protoss_Gateway){
      if (u->isIdle() && Broodwar->canMake(BWAPI::UnitTypes::Protoss_Zealot, u)){
        u->train(BWAPI::UnitTypes::Protoss_Zealot);
      }
    }
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

void ExampleAIModule::onUnitCreate(BWAPI::Unit unit)
{
    for (auto manager : _managers)
    {
      manager->onUnitCreate(unit);
    }
}

void ExampleAIModule::onUnitDestroy(BWAPI::Unit unit)
{
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
  for (auto manager : _managers)
  {
    manager->onUnitComplete(unit);
  }
}
