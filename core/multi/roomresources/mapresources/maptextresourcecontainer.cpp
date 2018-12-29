#include "maptextresourcecontainer.h"
#include "maptextresource.h"

PlayerRole MapTextResourceContainer::requiredEditStatus()
{
  return ROLE_PLAYER;
}

Resource *MapTextResourceContainer::newResource(const QJsonObject &data)
{
  return new MapTextResource(data);
}

const QString &MapTextResourceContainer::name() const
{
  return MapTextResource::resourceName;
}

MapTextResourceContainer::MapTextResourceContainer(QObject *parent) : ResourceContainer(parent)
{

}

MapTextResource *MapTextResourceContainer::getTextResource(int id)
{
  return (MapTextResource*) getResourceById(id);
}
