#include "roomvariablecontainercollection.h"
#include "core/multi/message/message.h"
#include "core/multi/message/roomvariablemessage.h"

MapSettingsVariableContainer *RoomVariableContainerCollection::getMapSettingsContainer() const
{
  return mapSettingsContainer;
}

void RoomVariableContainerCollection::setMapSettingsContainer(MapSettingsVariableContainer *value)
{
  mapSettingsContainer = value;
}

orpg::RoomVariableMessage RoomVariableContainerCollection::setMessage(const QString &variableName)
{
  RoomVariableContainer* container = containerMap[variableName];
  if(container!=nullptr)
  {
    return container->setMessage();
  }
  else
  {
    return orpg::RoomVariableMessage();
  }
}

QList<orpg::Message> RoomVariableContainerCollection::getAllVariableMessages()
{
  QList<orpg::Message> messages;
  foreach(RoomVariableContainer* container, containerMap)
  {
    orpg::RoomVariableMessage message = container->setMessage();
    if(message.isValid())
    {
      messages.append(message);
    }
  }
  return messages;
}

void RoomVariableContainerCollection::addContainer(RoomVariableContainer * container)
{
  container->setParent(this);
  containerMap.insert(container->name(),container);
  connect(container,&RoomVariableContainer::variableChanged,this,&RoomVariableContainerCollection::variableChangedInternal);
}

RoomVariableContainerCollection::RoomVariableContainerCollection(QObject *parent) : QObject(parent)
{
  mapSettingsContainer = new MapSettingsVariableContainer(this);
  addContainer(mapSettingsContainer);
}

void RoomVariableContainerCollection::variableChangedInternal()
{
  RoomVariableContainer* container = static_cast<RoomVariableContainer*>(sender());
  emit variableChanged(container->name());
}

bool RoomVariableContainerCollection::handleMessage(const orpg::RoomVariableMessage &msg)
{
  QString resourceName = msg.name();
  if(containerMap.contains(resourceName))
  {
    return containerMap[resourceName]->handleMessage(msg);
  }
  else
  {
    GenericRoomVariableContainer* container = new GenericRoomVariableContainer(resourceName,this);
    addContainer(container);
    return container->handleMessage(msg);
  }
}

void RoomVariableContainerCollection::announceVariables()
{

  foreach(auto container, containerMap.values())
    container->forceAnnounceChange();
}

void RoomVariableContainerCollection::clear()
{
  foreach(auto container, containerMap.values())
    container->reset();
}
