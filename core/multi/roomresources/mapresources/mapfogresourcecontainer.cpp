#include "mapfogresourcecontainer.h"

PlayerRole MapFogResourceContainer::requiredEditStatus()
{
  return ROLE_GM;
}

Resource *MapFogResourceContainer::newResource(const QJsonObject &data)
{
  return new MapFogResource(data);
}

void MapFogResourceContainer::afterSetting(const Resource &resource)
{
  const MapFogResource& res = static_cast<const MapFogResource&>(resource);
  positionMap.insert(res.getCenter(),res.getID());
}

void MapFogResourceContainer::beforeRemoving(int resId)
{
  positionMap.remove(getFogResource(resId)->getCenter());
}

bool MapFogResourceContainer::validForContainer(const Resource &resource)
{
  const MapFogResource& res = static_cast<const MapFogResource&>(resource);
  return !positionMap.contains(res.getCenter());
}

const QString &MapFogResourceContainer::name() const
{
  return MapFogResource::resourceName;
}

MapFogResourceContainer::MapFogResourceContainer(QObject *parent)
  : ResourceContainer(parent)
{

}

MapFogResource *MapFogResourceContainer::getFogResource(int id)
{
  return (MapFogResource*)getResourceById(id);
}
