#include "mapminiatureresourcecontainer.h"
#include "mapminiatureresource.h"

PlayerRole MapMiniatureResourceContainer::requiredEditStatus()
{
  return ROLE_PLAYER;
}

Resource *MapMiniatureResourceContainer::newResource(const QJsonObject &data)
{
  return new MapMiniatureResource(data);
}

const QString &MapMiniatureResourceContainer::name() const
{
  return MapMiniatureResource::resourceName;
}

MapMiniatureResourceContainer::MapMiniatureResourceContainer(QObject *parent) : ResourceContainer(parent)
{

}

MapMiniatureResource *MapMiniatureResourceContainer::getMiniatureResource(int id)
{
  return (MapMiniatureResource*) getResourceById(id);
}
