#include "maplineresourcecontainer.h"
#include "maplineresource.h"

PlayerRole MapLineResourceContainer::requiredEditStatus()
{
  return ROLE_PLAYER;
}

Resource *MapLineResourceContainer::newResource(const QJsonObject &data)
{
  return new MapLineResource(data);
}

const QString &MapLineResourceContainer::name() const
{
  return MapLineResource::resourceName;
}

MapLineResourceContainer::MapLineResourceContainer(QObject *parent)
  : ResourceContainer(parent)
{

}

MapLineResource* MapLineResourceContainer::getLineResource(int id)
{
  return (MapLineResource*)getResourceById(id);
}
