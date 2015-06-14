#include "TerrainAnalyzer.h"

using namespace std;
using namespace BWTA;
using namespace BWAPI;

TerrainAnalyzer* TerrainAnalyzer::_instance = nullptr;

DWORD __stdcall BeginAnalysisInThread()
{
  TerrainAnalyzer::Instance()->_Analyze();
  return 1;
}

TerrainAnalyzer::TerrainAnalyzer()
{

  CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BeginAnalysisInThread, 0, 0, NULL);
  _analyzed = false;
  _analysisJustFinished = false;
}

TerrainAnalyzer* TerrainAnalyzer::Instance()
{
  if (!_instance)
  {
    _instance = new TerrainAnalyzer();
  }
  return _instance;
}

void TerrainAnalyzer::_Analyze()
{
  analyze();
  _analyzed = true;
  _analysisJustFinished = true;
}

Chokepoint* TerrainAnalyzer::GetNearestChokepoint(Position position)
{
  if (_analyzed)
  {
    return getNearestChokepoint(position);
  }
  return nullptr;
}

Chokepoint* TerrainAnalyzer::GetNearestChokepoint(TilePosition tilePosition)
{
  if (_analyzed)
  {
    return getNearestChokepoint(tilePosition);
  }
  return nullptr;
}

set<Chokepoint*> TerrainAnalyzer::GetChokepoints()
{
  if (_analyzed)
  {
    return getChokepoints();
  }
  return set<Chokepoint*>();
}

BaseLocation* TerrainAnalyzer::GetNearestBaseLocation(Position position)
{
  if (_analyzed)
  {
    return getNearestBaseLocation(position);
  }
  return nullptr;
}

BaseLocation* TerrainAnalyzer::GetNearestBaseLocation(TilePosition tilePosition)
{
  if (_analyzed)
  {
    return getNearestBaseLocation(tilePosition);
  }
  return nullptr;
}

set<BaseLocation*> TerrainAnalyzer::GetStartLocations()
{
  if (_analyzed)
  {
    return getStartLocations();
  }
  return set<BaseLocation*>();
}

set<BaseLocation*> TerrainAnalyzer::GetBaseLocations()
{
  if (_analyzed)
  {
    return getBaseLocations();
  }
  return set<BaseLocation*>();
}

TilePosition TerrainAnalyzer::GetMyBaseLocation()
{
  return Broodwar->self()->getStartLocation();
}

double TerrainAnalyzer::GetGroundDistance(TilePosition start, TilePosition end)
{
  return getGroundDistance(start, end);
}

void TerrainAnalyzer::DrawTerrainDataWhenAble()
{
#ifdef DRAWING_ENABLED
  if (_analyzed)
  {
    for (std::set<BWTA::BaseLocation*>::const_iterator i = BWTA::getBaseLocations().begin(); i != BWTA::getBaseLocations().end(); i++)
    {
      TilePosition p = (*i)->getTilePosition();
      Position c = (*i)->getPosition();

      Broodwar->drawBoxMap(p.x * 32, p.y * 32, p.x * 32 + 4 * 32, p.y * 32 + 3 * 32, Colors::Blue);

      for (Unitset::iterator j = (*i)->getStaticMinerals().begin(); j != (*i)->getStaticMinerals().end(); j++)
      {
        Position q = (*j)->getInitialPosition();
        Broodwar->drawCircleMap(q.x, q.y, 30, Colors::Cyan);
      }

      for (Unitset::iterator j = (*i)->getGeysers().begin(); j != (*i)->getGeysers().end(); j++)
      {
        TilePosition q = (*j)->getInitialTilePosition();
        Broodwar->drawBoxMap(q.x * 32, q.y * 32, q.x * 32 + 4 * 32, q.y * 32 + 2 * 32, Colors::Orange);
      }

      if ((*i)->isIsland())
        Broodwar->drawCircleMap(c, 80, Colors::Yellow);
    }

    for (std::set<BWTA::Region*>::const_iterator r = BWTA::getRegions().begin(); r != BWTA::getRegions().end(); r++)
    {
      BWTA::Polygon p = (*r)->getPolygon();
      for (int j = 0; j<(int)p.size(); j++)
      {
        Position point1 = p[j];
        Position point2 = p[(j + 1) % p.size()];
        Broodwar->drawLineMap(point1, point2, Colors::Green);
      }
    }

    for (std::set<BWTA::Region*>::const_iterator r = BWTA::getRegions().begin(); r != BWTA::getRegions().end(); r++)
    {
      for (std::set<BWTA::Chokepoint*>::const_iterator c = (*r)->getChokepoints().begin(); c != (*r)->getChokepoints().end(); c++)
      {
        Position point1 = (*c)->getSides().first;
        Position point2 = (*c)->getSides().second;
        Broodwar->drawLineMap(point1, point2, Colors::Red);
      }
    }
  }
#endif
}
