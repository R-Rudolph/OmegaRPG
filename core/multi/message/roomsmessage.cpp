#include "roomsmessage.h"
#include<QJsonArray>

namespace orpg
{
  const QString RoomsMessage::messageType = "rooms";
  const QString RoomsMessage::roomsField = "rooms";

  RoomsMessage::RoomsMessage(const QList<RoomInfo>& rooms) : AbstractMessage()
  {
    _rooms = rooms;
  }

  RoomsMessage::RoomsMessage(const QJsonObject& object) : AbstractMessage()
  {
    QJsonArray roomArray = object[roomsField].toArray();
    foreach(QJsonValue value, roomArray)
    {
      RoomInfo info(value.toObject());
      if(info.isValid())
        _rooms.append(info);
    }
  }

  QJsonObject RoomsMessage::toJson() const
  {
    QJsonObject object;
    addToJson(object,actionField,messageType);
    QJsonArray roomArray;
    foreach(const RoomInfo& room, _rooms)
    {
      roomArray.append(room.toJson());
    }
    object.insert(roomsField,roomArray);
    return object;
  }

  bool RoomsMessage::isValid() const
  {
    return true;
  }

  BaseMessage* RoomsMessage::clone() const
  {
    return new RoomsMessage(*this);
  }

  BaseMessage::MessageType RoomsMessage::type() const
  {
    return TypeRooms;
  }

  QList<RoomInfo> RoomsMessage::rooms() const
  {
    return _rooms;
  }

  void RoomsMessage::setRooms(const QList<RoomInfo>& rooms)
  {
    _rooms = rooms;
  }

}

