#include "initiativeresourcecontainer.h"
#include "genericresource.h"

const QString &InitiativeResourceContainer::name() const
{
  return InitiativeResource::initiativeName;
}

int InitiativeResourceContainer::findIdByPosition(int position, int excludeId)
{
  foreach(Resource* resource, resourceMap.values())
  {
    InitiativeResource* initRes = (InitiativeResource*) resource;
    if(initRes->getPosition()==position && initRes->getID()!=excludeId)
      return initRes->getID();
  }
  return -1;
}

Resource *InitiativeResourceContainer::newResource(const QJsonObject &data)
{
  return new InitiativeResource(data);
}

void InitiativeResourceContainer::afterSetting(const Resource &resource)
{
  const InitiativeResource& initresource = static_cast<const InitiativeResource&>(resource);
  //fix multiple items having same positions
  int position = initresource.getPosition();
  int id = findIdByPosition(position,resource.getID());
  while(id!=-1)
  {
    InitiativeResource* res = (InitiativeResource*) resourceMap[id];
    position++;
    res->setPosition(position);
    id = findIdByPosition(position,res->getID());
  }
  //fix multiple active items
  if(initresource.getIsActive())
  {
    foreach(Resource* it, resourceMap.values())
    {
      InitiativeResource* initIt = (InitiativeResource*) it;
      if(initIt->getID()!=initresource.getID())
      {
        initIt->setIsActive(false);
      }
    }
  }
}

InitiativeResourceContainer::InitiativeResourceContainer(QObject* parent)
  :ResourceContainer(parent)
{

}

QList<InitiativeResource> InitiativeResourceContainer::getResources() const
{
  QList<InitiativeResource> list;
  foreach(Resource* res,resourceMap.values())
  {
    list.push_back(*((InitiativeResource*)res));
  }
  std::sort(list.begin(),list.end());
  return list;
}

QList<InitiativeResource> InitiativeResourceContainer::getResourcesByInitiative() const
{
  QList<InitiativeResource> list;
  foreach(Resource* res,resourceMap.values())
  {
    list.push_back(*((InitiativeResource*)res));
  }
  std::sort(list.begin(),list.end(),InitiativeResource::initiativeGreater);
  return list;
}

orpg::RoomResourceMessage InitiativeResourceContainer::activateMessage(int id) const
{
  InitiativeResource initresource = *((InitiativeResource*)resourceMap[id]);
  initresource.setIsActive(true);
  return orpg::RoomResourceMessage::setResourceMessage(initresource);
}

orpg::RoomResourceMessage InitiativeResourceContainer::moveMessage(int id, int position)
{
  InitiativeResource initresource = *((InitiativeResource*)resourceMap[id]);
  initresource.setPosition(position);
  return orpg::RoomResourceMessage::setResourceMessage(initresource);
}

orpg::RoomResourceMessage InitiativeResourceContainer::sortMessage()
{
  QList<GenericResource> genericResources;
  QList<InitiativeResource> sortedList = getResourcesByInitiative();
  for(int i=0;i<sortedList.size();i++)
  {
    InitiativeResource& resource = sortedList[i];
    resource.setPosition(i);
    genericResources.append(resource);
  }
  return orpg::RoomResourceMessage::setResourceMessage(genericResources);
}

orpg::RoomResourceMessage InitiativeResourceContainer::nextMessage()
{
  QList<InitiativeResource> sortedList = getResources();
  for(int i=0;i<sortedList.size();i++)
  {
    if(sortedList[i].getIsActive())
    {
      InitiativeResource& res = sortedList[(i+1)%sortedList.size()];
      res.setIsActive(true);
      return orpg::RoomResourceMessage::setResourceMessage(res);
    }
  }
  if(sortedList.size()>0)
  {
    InitiativeResource& res = sortedList[0];
    res.setIsActive(true);
    return orpg::RoomResourceMessage::setResourceMessage(res);
  }
  return orpg::RoomResourceMessage();
}
