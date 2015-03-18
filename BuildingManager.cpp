#include "BuildingManager.h"

BuildingManager::BuildingManager()
{

}

void BuildingManager::AddBuildRequest(BuildTask BuildTask)
{
  _buildQueue.push(BuildTask);
}

void BuildingManager::Update()
{

}
