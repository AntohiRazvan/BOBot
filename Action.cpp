#include "Action.h"

char* Action::_action_string[] = { "None", "Build", "Train", "Attack" };

Action::Action()
{
}

char* Action::ToString(Actions action)
{
  return _action_string[action];
}

int Action::ToAction(const char* actionString)
{
  if (!strcmp(actionString, "None"))
  {
    return 0;
  }
  else if (!strcmp(actionString, "Build"))
  {
    return 1;
  }
  else if (!strcmp(actionString, "Train"))
  {
    return 2;
  }
  else if (!strcmp(actionString, "Attack"))
  {
    return 3;
  }
  else
  {
    return -1;
  }
}
