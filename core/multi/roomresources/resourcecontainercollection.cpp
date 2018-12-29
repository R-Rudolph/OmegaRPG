#include "resourcecontainercollection.h"

#include "core/multi/roomresources/resourcecontainer.h"
#include "core/multi/roomresources/initiativeresourcecontainer.h"
#include "core/multi/roomresources/genericresourcecontainer.h"
#include "core/multi/message/message.h"

#include "mapresources/maplineresourcecontainer.h"
#include "mapresources/mapfogresourcecontainer.h"
#include "mapresources/mapminiatureresourcecontainer.h"
#include "mapresources/maptextresourcecontainer.h"

InitiativeResourceContainer *ResourceContainerCollection::getInitiativeContainer() const
{
  return initiativeContainer;
}

MapLineResourceContainer *ResourceContainerCollection::getMapLineContainer() const
{
  return mapLineContainer;
}

MapFogResourceContainer *ResourceContainerCollection::getMapFogContainer() const
{
  return mapFogContainer;
}

MapMiniatureResourceContainer *ResourceContainerCollection::getMapMiniatureContainer() const
{
  return mapMiniatureContainer;
}

MapTextResourceContainer *ResourceContainerCollection::getMapTextContainer() const
{
  return mapTextContainer;
}

void ResourceContainerCollection::newContainer(ResourceContainer *container)
{
  container->setParent(this);
  containerMap.insert(container->name(),container);
  connect(container,&ResourceContainer::resourceChanged,this,&ResourceContainerCollection::resourceChangedInternal);
  connect(container,&ResourceContainer::resourceRemoved,this,&ResourceContainerCollection::resourceRemovedInternal);
}

ResourceContainerCollection::ResourceContainerCollection(QObject *parent) : QObject(parent)
{
  initiativeContainer = new InitiativeResourceContainer();
  newContainer(initiativeContainer);
  connect(initiativeContainer,&ResourceContainer::resourceChanged,this,&ResourceContainerCollection::initiativeChangedInternal);
  connect(initiativeContainer,&ResourceContainer::resourceRemoved,this,&ResourceContainerCollection::initiativeChangedInternal);
  mapLineContainer = new MapLineResourceContainer();
  newContainer(mapLineContainer);
  mapFogContainer = new MapFogResourceContainer();
  newContainer(mapFogContainer);
  mapMiniatureContainer = new MapMiniatureResourceContainer();
  newContainer(mapMiniatureContainer);
  mapTextContainer = new MapTextResourceContainer();
  newContainer(mapTextContainer);
}

QList<orpg::Message> ResourceContainerCollection::getAllResourceMessages()
{
  QList<orpg::Message> list;
  foreach(ResourceContainer* container, containerMap.values())
  {
    orpg::RoomResourceMessage message = container->allMessages();
    if(message.isValid())
    {
      list.append(message);
    }
  }
  return list;
}

void ResourceContainerCollection::clear()
{
  for(auto it = containerMap.begin();it!=containerMap.end();++it)
  {
    it.value()->clear();
  }
}

bool ResourceContainerCollection::handleMessage(const orpg::RoomResourceMessage &msg)
{
  QString resourceName = msg.name();
  if(containerMap.contains(resourceName))
  {
    return containerMap[resourceName]->handleMessage(msg);
  }
  else
  {
    GenericResourceContainer* container = new GenericResourceContainer(resourceName);
    newContainer(container);
    return container->handleMessage(msg);
  }
}

void ResourceContainerCollection::resourceChangedInternal(QList<int> ids)
{
  emit resourceChanged(static_cast<ResourceContainer*>(QObject::sender()),ids);
}

void ResourceContainerCollection::resourceRemovedInternal(QList<int> ids)
{
  emit resourceRemoved(static_cast<ResourceContainer*>(QObject::sender()),ids);
}

void ResourceContainerCollection::initiativeChangedInternal(QList<int>)
{
  auto resources = initiativeContainer->getResources();
  emit initiativeChanged(resources);
}
