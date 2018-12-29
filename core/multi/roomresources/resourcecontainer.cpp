#include "resourcecontainer.h"
#include "genericresource.h"

#include<QVector>
#include<QJsonArray>

const QString ResourceContainer::idField = "resID";
const QString ResourceContainer::actionField = "resAction";
const QString ResourceContainer::nameField = "resource";
const QString ResourceContainer::dataField = "data";
const QString ResourceContainer::setString = "set";
const QString ResourceContainer::removeString = "remove";

PlayerRole ResourceContainer::requiredEditStatus()
{
  return ROLE_GM;
}

void ResourceContainer::beforeSetting(const Resource &)
{

}

void ResourceContainer::afterSetting(const Resource &)
{

}

void ResourceContainer::beforeRemoving(int)
{

}

void ResourceContainer::afterRemoving(int)
{

}

QList<int> ResourceContainer::findNextIds(int n)
{
  QList<int> ids;
  if(reuseId)
  {
    int ctr = idOffset;
    auto it = resourceMap.begin();
    int itId;
    while(ids.size()<n)
    {
      if(it!=resourceMap.end())
      {
        itId = it.key();
        if(itId==ctr)
        {
          ++ctr;
          ++it;
        }
        else
        {
          ids.append(ctr);
          ++ctr;
        }
      }
      else
      {
        ids.append(ctr);
        ++ctr;
      }
    }
  }
  else
  {
    int start = qMax(idOffset,maxId);
    for(int i=start;i<start+n;i++)
      ids.append(i);
    maxId = qMax(idOffset,maxId+n);
  }
  return ids;
}

bool ResourceContainer::validForContainer(const Resource&)
{
  return true;
}

ResourceContainer::ResourceContainer(QObject *parent) : QObject(parent)
{
  idOffset = 0;
  maxId = 0;
  reuseId = true;
}

bool ResourceContainer::handleMessage(const orpg::RoomResourceMessage &msg)
{
  if((msg.senderRole()<requiredEditStatus()) && msg.hasSenderId())
    return false;
  if(msg.isNewMessage())
  {
    QVector<GenericResource> resources = msg.resources().toVector();
    QList<int> resIds = findNextIds(resources.size());
    for(int i=0;i<resources.size();++i)
    {
      resources[i].setID(resIds[i]);
    }
    return setResources(resources);
  }
  else if(msg.isSetMessage())
  {
    return setResources(msg.resources().toVector());
  }
  else if(msg.isRemoveMessage())
  {
    return removeResources(msg.ids());
  }
  return false;
}

bool ResourceContainer::setResources(const QVector<GenericResource>& resources)
{
  QList<int> changedResources;
  for(int i=0;i<resources.size();i++)
  {
    const GenericResource& gResource = resources[i];
    Resource* res = this->newResource(gResource.data());
    res->setID(gResource.getID());
    if(res->isValid() && validForContainer(*res))
    {
      changedResources.append(gResource.getID());
      beforeSetting(*res);
      if(resourceMap.contains(gResource.getID()))
      {
        delete resourceMap[gResource.getID()];
        resourceMap.remove(gResource.getID());
      }
      resourceMap.insert(gResource.getID(),res);
      afterSetting(*res);
    }
  }
  if(changedResources.size()>0)
  {
    emit resourceChanged(changedResources);
  }
  return true;
}

bool ResourceContainer::removeResources(const QList<int>& resIds)
{
  QList<int> removedResources;
  foreach(int resId, resIds)
  {
    if(resourceMap.contains(resId))
    {
      beforeRemoving(resId);
      delete resourceMap[resId];
      resourceMap.remove(resId);
      afterRemoving(resId);
      removedResources.append(resId);
    }
  }
  if(removedResources.size()>0)
    emit resourceRemoved(removedResources);
  return true;
}

void ResourceContainer::clear()
{
  QList<int> keys;
  for(auto it = resourceMap.begin();it!=resourceMap.end();++it)
  {
    delete it.value();
    keys.append(it.key());
  }
  resourceMap.clear();
  if(keys.size())
    emit resourceRemoved(keys);
  maxId = 0;
}

orpg::RoomResourceMessage ResourceContainer::removeMessage(int id) const
{
  return orpg::RoomResourceMessage::removeResourceMessage(name(),id);
}

orpg::RoomResourceMessage ResourceContainer::removeMessage(const QList<int> &ids) const
{
  return orpg::RoomResourceMessage::removeResourceMessage(name(),ids);
}

orpg::RoomResourceMessage ResourceContainer::setMessage(int id) const
{
  return setMessage(*resourceMap[id]);
}

orpg::RoomResourceMessage ResourceContainer::setMessage(const QList<int>& ids) const
{
  QList<GenericResource> genericResources;
  foreach(int id, ids)
  {
    if(resourceMap.contains(id))
    {
      genericResources.append(*resourceMap[id]);
    }
  }
  return orpg::RoomResourceMessage::setResourceMessage(genericResources);
}

orpg::RoomResourceMessage ResourceContainer::setMessage(const Resource &resource) const
{
  return orpg::RoomResourceMessage::setResourceMessage(resource);
}

orpg::RoomResourceMessage ResourceContainer::setMessage(const QList<GenericResource> &resources) const
{
  return orpg::RoomResourceMessage::setResourceMessage(resources);
}

orpg::RoomResourceMessage ResourceContainer::newMessage(const Resource &resource) const
{
  return orpg::RoomResourceMessage::newResourceMessage(resource);
}

orpg::RoomResourceMessage ResourceContainer::newMessage(const QList<GenericResource> &resources) const
{
  QJsonObject msg;
  msg.insert("action","resource");
  msg.insert(nameField,this->name());
  msg.insert(actionField,setString);
  QJsonArray dataArray;
  foreach(const GenericResource& resource, resources)
  {
    dataArray.append(resource.data());
  }
  msg.insert(dataField,dataArray);
  return msg;
}

Resource *ResourceContainer::getResourceById(int id)
{
  return resourceMap[id];
}

orpg::RoomResourceMessage ResourceContainer::allMessages()
{
  return setMessage(resourceMap.keys());
}

orpg::RoomResourceMessage ResourceContainer::allRemoveMessages()
{
  return removeMessage(resourceMap.keys());
}
