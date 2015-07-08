#include "BuildOrder.h"

using namespace std;
using namespace BWAPI;

BuildOrder::BuildOrder(BuildingManager *bm, ProductionManager *pm, ArmyManager *am)
{
  _buildingManager = bm;
  _productionManager = pm;
  _armyManager = am;
  ReadBuildOrder();
}


void BuildOrder::ReadBuildOrder()
{
  ifstream in(_fileName);
  while (in.good())
  {
    Item *item = new Item();
    in >> *item;
    _buildOrder.push(item);
    
    if (item->GetAction() == Build)
    {
      UnitType unit = item->GetParameters()[0];
      int count = item->GetParameters()[1];

    }
  }
  in.close();
}

void BuildOrder::update()
{
  if (Broodwar->getFrameCount() > 100000)
  {
    Broodwar->leaveGame();
  }

  if (_buildOrder.empty())
  {
    return;
  }

  if (!_itemStarted)
  {
    _itemStarted = true;
    Item *item = _buildOrder.front();
    UnitType unit;
    int count;

    switch (item->GetAction())
    {
      case Build:
      {
        unit = item->GetParameters()[0];
        count = item->GetParameters()[1];
        for (int i = 0; i < count; i++)
        {
          _buildingManager->AddBuildRequest(unit);
        } 
        break;
      }
      case Train:
      {
        unit = item->GetParameters()[0];
        count = item->GetParameters()[1];
        if (count != 0)
        {
          _productionManager->Train(unit, count);
        }
        else
        {
          _productionManager->Train(unit);
          _buildOrder.pop();
          _itemStarted = false;
        }
        break;
      }
      case Attack:
      {
        _armyManager->Attack();
        _buildOrder.pop();
        _itemStarted = false;
      }
    }
  }
}

void BuildOrder::onUnitComplete(Unit unit)
{
  if (Broodwar->getFrameCount() < 5)
  {
    return;
  }
  if (unit->getType().isBuilding())
  {
    return;
  }
  if (_buildOrder.empty())
  {
    return;
  }

  auto it = _unitsMade.find(unit->getType());
  if (it != _unitsMade.end())
  {
    it->second += 1;
  }
  else
  {
    _unitsMade.insert(pair<BWAPI::UnitType, int>(unit->getType(), 1));
  }

  Item *item = _buildOrder.front();
  UnitType unitType;
  int count;

  switch (item->GetAction())
  {
    case Build:
    {
      break;
    }
    case Train:
    {
      unitType = item->GetParameters()[0];
      count = item->GetParameters()[1];
      auto p = _unitsMade.find(unitType);
      if (p != _unitsMade.end())
      {
        if (count - p->second < 0)
        {
          p->second -= count;
          _buildOrder.pop();
          _itemStarted = false;
        }
        else if (count - p->second == 0)
        {
          _unitsMade.erase(p);
          _buildOrder.pop();
          _itemStarted = false;
        }
      }
      break;
    }
  }
}

void BuildOrder::onUnitCreate(Unit unit)
{
  if (_buildOrder.empty())
  {
    return;
  }
  if (!Filter::IsBuilding(unit))
  {
    return;
  }
  auto it = _unitsMade.find(unit->getType());
  if (it != _unitsMade.end())
  {
    it->second += 1;
  }
  else
  {
    _unitsMade.insert(pair<BWAPI::UnitType, int>(unit->getType(), 1));
  }

  Item *item = _buildOrder.front();
  UnitType unitType;
  int count;

  if (item->GetAction() == Build)
  {
    unitType = item->GetParameters()[0];
    count = item->GetParameters()[1];
    auto p = _unitsMade.find(unitType);
    if (p != _unitsMade.end())
    {
      if (count - p->second < 0)
      {
        p->second -= count;
        _buildOrder.pop();
        _itemStarted = false;
      }
      else if (count - p->second == 0)
      {
        _unitsMade.erase(p);
        _buildOrder.pop();
        _itemStarted = false;
      }
    }
  }
}

void BuildOrder::onUnitMorph(BWAPI::Unit unit)
{
  if (_buildOrder.empty())
  {
    return;
  }

  auto it = _unitsMade.find(unit->getType());
  if (it != _unitsMade.end())
  {
    it->second += 1;
  }
  else
  {
    _unitsMade.insert(pair<BWAPI::UnitType, int>(unit->getType(), 1));
  }

  Item *item = _buildOrder.front();
  UnitType unitType;
  int count;

  switch (item->GetAction())
  {
    case Build:
    case Train:
    {
      unitType = item->GetParameters()[0];
      count = item->GetParameters()[1];
      auto p = _unitsMade.find(unitType);
      if (p != _unitsMade.end())
      {
        if (count - p->second < 0)
        {
          p->second -= count;
          _buildOrder.pop();
          _itemStarted = false;
        }
        else if (count - p->second == 0)
        {
          _unitsMade.erase(p);
          _buildOrder.pop();
          _itemStarted = false;
        }
      }
      break;
    }
  }
}
