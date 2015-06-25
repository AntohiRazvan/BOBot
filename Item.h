#pragma once
#include <vector>
#include "Action.h"

class Item
{
public:
  Actions _action;
  std::vector<int> _parameters;
public:
  Item();
  void SetAction(Actions action);
  Actions GetAction();

  void AddParameter(int param);
  std::vector<int> GetParameters();

  friend std::ostream& operator<<(std::ostream& os, const Item& item);
  friend std::istream& operator>>(std::istream& is, Item& item);
  friend bool operator==(const Item& item1, const Item& item2);
};
