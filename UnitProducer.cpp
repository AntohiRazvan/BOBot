#include "UnitProducer.h"

using namespace std;
using namespace BWAPI;

UnitProducer::UnitProducer(ResourceManager *rm)
{
  _resourceManager = rm;
  _halted = false;
  _mainFocus = NULL;
  _tempFocus = NULL;
  _tempFocusCount = 0;
}

void UnitProducer::Produce()
{
  if (_halted)
  {
    return;
  }

  for (auto p : _producers)
  {
    if (p->isTraining())
    {
      continue;
    }
    
    if ((_tempFocusCount >= 0) && (_tempFocus))
    {
      if (_resourceManager->CanAfford(_tempFocus) && (p->canTrain(_tempFocus)))
      {
        p->train(_tempFocus);
        _tempFocusCount--;
      }
    }
    else
    {
      if ((_mainFocus) && (p->canTrain(_mainFocus)))
      {
        if (_resourceManager->CanAfford(_mainFocus))
        {
          p->train(_mainFocus);
        }
      }
    }
  }
}

void UnitProducer::AddProducer(Unit unit)
{
  _producers.push_back(unit);
}
void UnitProducer::RemoveProducer(Unit unit)
{
  vector<Unit>::iterator it = find(_producers.begin(), _producers.end(), unit);
  if (it != _producers.end())
  {
    _producers.erase(it);
  }
}

void UnitProducer::SetFocus(UnitType unit)
{
  _mainFocus = unit;
}

void UnitProducer::SetFocus(UnitType unit, int count)
{
  _tempFocus = unit;
  _tempFocusCount = count;
}

void UnitProducer::Halt()
{
  _halted = true;
}

void UnitProducer::Resume()
{
  _halted = false;
}
