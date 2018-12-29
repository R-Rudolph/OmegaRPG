#include "resource.h"

int Resource::getID() const
{
  return id;
}

void Resource::setID(int value)
{
  id = value;
}

Resource::Resource()
{
  id = -1;
  valid = false;
}

Resource::~Resource()
{

}

bool Resource::isValid() const
{
  return valid;
}
