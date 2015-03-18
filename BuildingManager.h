#pragma once
#include <queue>
#include <vector>
#include "BuildTask.h"

struct CompareBuildTaskPriority 
{
  bool operator()(const BuildTask &BuildTask1, const BuildTask &BuildTask2)
  {
    return BuildTask1.getPriority() > BuildTask2.getPriority();
  }
};

class BuildingManager
{
  std::priority_queue<BuildTask, std::vector<BuildTask>, CompareBuildTaskPriority> _buildQueue;
public:
  BuildingManager();
  void AddBuildRequest(BuildTask BuildTask);
  void Update();
};

