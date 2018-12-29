#include "room.h"
#include "core/multi/roomresources/resourcecontainer.h"
#include "core/multi/roompropertiesutility.h"

namespace orpg
{

  void Room::sendMessageInternal(Message msg)
  {
    msg.setRoom(QString::number(_id));
    emit sendMessage(msg);
  }

  Room::Room(QObject *parent) : QObject(parent)
  {
    _time = QDateTime::currentDateTime();
    resources = new ResourceContainerCollection(this);
    connect(resources,&ResourceContainerCollection::resourceChanged,this,&Room::resourceChanged);
    connect(resources,&ResourceContainerCollection::resourceRemoved,this,&Room::resourceRemoved);
    variables = new RoomVariableContainerCollection(this);
    connect(variables,&RoomVariableContainerCollection::variableChanged,this,&Room::variableChanged);
    this->_numPlayers = 0;
  }

  Room::Room(int id,
             QString name,
             QString password,
             QString dmpassword,
             QObject *parent,
             const QString &propertiesFilepath) : Room(parent)
  {
    this->_propertiesFilepath = propertiesFilepath;
    if(!propertiesFilepath.isEmpty())
    {
      RoomPropertiesUtility::loadRoomProperties(variables,resources,propertiesFilepath);
    }
    this->_id = id;
    this->_name = name;
    this->_password = password;
    this->_dmpassword = dmpassword;
  }

  Room::~Room()
  {
    if(!_propertiesFilepath.isEmpty())
    {
      RoomPropertiesUtility::saveRoomProperties(variables,resources,_propertiesFilepath);
    }
  }

  RoomInfo Room::info() const
  {
    return RoomInfo(_name,!_password.isEmpty(),_id,_numPlayers,_time);
  }

  void Room::resourceChanged(ResourceContainer *container, QList<int> ids)
  {
    sendMessageInternal(container->setMessage(ids));
  }

  void Room::resourceRemoved(ResourceContainer *container, QList<int> ids)
  {
    sendMessageInternal(container->removeMessage(ids));
  }

  void Room::variableChanged(const QString &variableName)
  {
    sendMessageInternal(Message(variables->setMessage(variableName)));
  }

}
