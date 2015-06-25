#include "BuildOrder.h"

using namespace std;
using namespace BWAPI;

BuildOrder::BuildOrder(BuildingManager *bm, ProductionManager *pm)
{
  _buildingManager = bm;
  _productionManager = pm;
  ReadBuildOrder();
}


void BuildOrder::ReadBuildOrder()
{
  ifstream in(_fileName);
  while (in.good())
  {
    Item *item = new Item();
    in >> *item;
    
    BWAPI::UnitType unit;
    int count;
    switch (item->GetAction())
    {
      case 1:
      {
        unit = item->GetParameters()[0];
        count = item->GetParameters()[1];
        for (int i = 0; i < count; i++)
        {
          _buildingManager->AddBuildRequest(unit);
        }
        break;
      }
      case 2:
      {
         unit = item->GetParameters()[0];
         count = item->GetParameters()[1];
         if (count == 0)
         {
           _productionManager->Train(unit);
         }
         else
         {
           _productionManager->Train(unit, count);
         }
         break;
      case 3:
      {
         //TODO: Add attack support
      }
      }

    }

  }
}
