#pragma once
#include <BWTA.h>
#include <BWTA/Chokepoint.h>
#include <BWTA/BaseLocation.h>
#include <BWAPI.h>
#include <Windows.h>
#include <set>

#define DRAWING_ENABLED

class TerrainAnalyzer
{
  TerrainAnalyzer();
  static TerrainAnalyzer *_instance;

  bool _analyzed;
  bool _analysisJustFinished;

  void _Analyze();
  friend DWORD __stdcall BeginAnalysisInThread();

public:
  static TerrainAnalyzer* Instance();
  void DrawTerrainDataWhenAble();

  bool BaseIsMine(BWAPI::Position baseLocation);
  bool Analyzed();
  BWTA::Chokepoint* GetNearestChokepoint(BWAPI::Position position);
  BWTA::Chokepoint* GetNearestChokepoint(BWAPI::TilePosition position);
  std::set<BWTA::Chokepoint*> GetChokepoints();
  BWTA::BaseLocation* GetNearestBaseLocation(BWAPI::Position position);
  BWTA::BaseLocation* GetNearestBaseLocation(BWAPI::TilePosition tilePosition);
  BWTA::BaseLocation* GetMyBaseLocation();
  std::set<BWTA::BaseLocation*> GetStartLocations();
  std::set<BWTA::BaseLocation*> GetBaseLocations();
  double GetGroundDistance(BWAPI::TilePosition start, BWAPI::TilePosition end);
  BWAPI::TilePosition GetNearestExpansionLocation(BWTA::BaseLocation* position);
};
