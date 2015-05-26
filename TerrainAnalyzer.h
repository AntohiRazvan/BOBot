#pragma once
#include <BWTA.h>
#include <BWTA/Chokepoint.h>
#include <BWTA/BaseLocation.h>
#include <BWAPI.h>
#include <Windows.h>
#include <set>

class TerrainAnalyzer
{
  TerrainAnalyzer();
  static TerrainAnalyzer *_instance;

  bool _analyzed;
  bool _analysisJustFinished;

  void _Analyze();

public:
  static TerrainAnalyzer* Instance();
  void DrawTerrainDataWhenAble();

  BWTA::Chokepoint* GetNearestChokepoint(BWAPI::Position position);
  std::set<BWTA::Chokepoint*> GetChokepoints();
  BWTA::BaseLocation* GetNearestBaseLocation(BWAPI::Position position);
  BWTA::BaseLocation* GetNearestBaseLocation(BWAPI::TilePosition tilePosition);
  std::set<BWTA::BaseLocation*> GetStartLocations();
  std::set<BWTA::BaseLocation*> GetBaseLocations();
  double GetGroundDistance(BWAPI::TilePosition start, BWAPI::TilePosition end);
  friend DWORD __stdcall BeginAnalysisInThread();
};