#include "roomresourcemessage.h"
#include<QJsonArray>

namespace orpg
{
  const QString RoomResourceMessage::messageType         = "resource";
  const QString RoomResourceMessage::resourceActionField = "resAction";
  const QString RoomResourceMessage::resourceIdField     = "resID";
  const QString RoomResourceMessage::nameField           = "resource";
  const QString RoomResourceMessage::dataField           = "data";

  const QString RoomResourceMessage::setString    = "set";
  const QString RoomResourceMessage::removeString = "remove";

  const QMap<RoomResourceMessage::ActionType,QString> RoomResourceMessage::actionStringMap({
                                                                                       std::pair<RoomResourceMessage::ActionType,QString>(RoomResourceMessage::ActionSet,RoomResourceMessage::setString),
                                                                                       std::pair<RoomResourceMessage::ActionType,QString>(RoomResourceMessage::ActionRemove,RoomResourceMessage::removeString)
                                                                                           });
  const QMap<QString,RoomResourceMessage::ActionType> RoomResourceMessage::stringActionMap({
                                                                                       std::pair<QString,RoomResourceMessage::ActionType>(RoomResourceMessage::setString,RoomResourceMessage::ActionSet),
                                                                                       std::pair<QString,RoomResourceMessage::ActionType>(RoomResourceMessage::removeString,RoomResourceMessage::ActionRemove)
                                                                                           });

  RoomResourceMessage::ActionType RoomResourceMessage::action() const
  {
    return _action;
  }

  QString RoomResourceMessage::name() const
  {
    return _name;
  }

  QList<int> RoomResourceMessage::ids() const
  {
    return _ids;
  }

  QList<GenericResource> RoomResourceMessage::resources() const
  {
    QList<GenericResource> resources;
    foreach(const QJsonValue& data, _data)
    {
      resources.append(GenericResource(data.toObject()));
    }
    for(int i=0;i<_ids.size() && i<resources.size();++i)
    {
      resources[i].setID(_ids[i]);
    }
    return resources;
  }

  bool RoomResourceMessage::isNewMessage() const
  {
    return _action == ActionSet && _ids.isEmpty();
  }

  bool RoomResourceMessage::isSetMessage() const
  {
    return _action == ActionSet && !_ids.isEmpty();
  }

  bool RoomResourceMessage::isRemoveMessage() const
  {
    return _action == ActionRemove;
  }

  RoomResourceMessage::RoomResourceMessage() : AbstractMessage()
  {
    _isValid = false;
    _action = ActionUnknown;
  }

  RoomResourceMessage RoomResourceMessage::newResourceMessage(const Resource& resource)
  {
    QList<GenericResource> resources;
    resources.append(resource);
    return newResourceMessage(resources);
  }

  RoomResourceMessage RoomResourceMessage::newResourceMessage(const QList<GenericResource>& resources)
  {
    RoomResourceMessage message;
    message._action = ActionSet;
    if(resources.size()>0)
    {
      message._name = resources.first().name();
      message._isValid = true;
    }
    foreach(const GenericResource& resource, resources)
    {
      message._data.append(resource.data());
      if(message._name != resource.name())
        message._isValid = false;
    }
    return message;
  }

  RoomResourceMessage RoomResourceMessage::setResourceMessage(const Resource& resource)
  {
    QList<GenericResource> resources;
    resources.append(resource);
    return setResourceMessage(resources);
  }

  RoomResourceMessage RoomResourceMessage::setResourceMessage(const QList<GenericResource>& resources)
  {
    RoomResourceMessage message = newResourceMessage(resources);
    foreach(const GenericResource& resource, resources)
    {
      message._ids.append(resource.getID());
    }
    return message;
  }

  RoomResourceMessage RoomResourceMessage::removeResourceMessage(const Resource& resource)
  {
    QList<GenericResource> resources;
    resources.append(resource);
    return removeResourceMessage(resources);
  }

  RoomResourceMessage RoomResourceMessage::removeResourceMessage(const QList<GenericResource>& resources)
  {
    if(resources.size()==0)
      return RoomResourceMessage();
    QList<int> ids;
    foreach(const GenericResource& resource, resources)
    {
      ids.append(resource.getID());
    }
    return removeResourceMessage(resources.first().name(),ids);
  }

  RoomResourceMessage RoomResourceMessage::removeResourceMessage(const QString& resourceName, const QList<int>& ids)
  {
    RoomResourceMessage message;
    message._action = ActionRemove;
    message._name = resourceName;
    message._ids = ids;
    message._isValid = true;
    return message;
  }

  RoomResourceMessage RoomResourceMessage::removeResourceMessage(const QString& resourceName, int id)
  {
    QList<int> ids;
    ids.append(id);
    return removeResourceMessage(resourceName,ids);
  }

  RoomResourceMessage::RoomResourceMessage(const QJsonObject& object) : AbstractMessage(object)
  {
    _isValid = true;
    _name = object[nameField].toString();
    if(_name.isEmpty())
    {
      _isValid = false;
    }
    if(stringActionMap.contains(object[resourceActionField].toString()))
      _action = stringActionMap[object[resourceActionField].toString()];
    else
    {
      _isValid = false;
    }
    {
      QJsonArray idArray = object[resourceIdField].toArray();
      foreach(const QJsonValue& value, idArray)
      {
        _ids.append(value.toInt());
      }
    }
    {
      QJsonArray dataArray = object[dataField].toArray();
      foreach(const QJsonValue& value, dataArray)
      {
        _data.append(value.toObject());
      }
    }

  }

  QJsonObject RoomResourceMessage::toJson() const
  {
    QJsonObject object = AbstractMessage::toJson();
    addToJson(object,actionField,messageType);
    addToJson(object,nameField,_name);
    {
      QJsonArray idArray;
      foreach(int id, _ids)
        idArray.append(id);
      addToJson(object,resourceIdField,idArray);
    }
    {
      QJsonArray dataArray;
      foreach(const QJsonObject& dataEntry, _data)
        dataArray.append(dataEntry);
      addToJson(object,dataField,dataArray);
    }
    addToJson(object,resourceActionField,actionStringMap[_action]);
    return object;
  }

  bool RoomResourceMessage::isValid() const
  {
    return _isValid;
  }

  BaseMessage* RoomResourceMessage::clone() const
  {
    return new RoomResourceMessage(*this);
  }

  BaseMessage::MessageType RoomResourceMessage::type() const
  {
    return TypeRoomResource;
  }

}

