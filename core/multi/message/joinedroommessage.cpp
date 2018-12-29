#include "joinedroommessage.h"

namespace orpg
{
  const QString JoinedRoomMessage::messageType = "joinedRoom";
  const QString JoinedRoomMessage::roomField = "room";

  RoomInfo JoinedRoomMessage::roomInfo() const
  {
    return _roomInfo;
  }

  void JoinedRoomMessage::setRoomInfo(const RoomInfo& roomInfo)
  {
    _roomInfo = roomInfo;
  }

  JoinedRoomMessage::JoinedRoomMessage(const RoomInfo& roomInfo) : AbstractMessage()
  {
    _roomInfo = roomInfo;
  }

  JoinedRoomMessage::JoinedRoomMessage(const QJsonObject& object) : AbstractMessage()
  {
    _roomInfo = RoomInfo(object[roomField].toObject());
  }

  QJsonObject JoinedRoomMessage::toJson() const
  {
    QJsonObject object;
    addToJson(object,actionField,messageType);
    addToJson(object,roomField,_roomInfo.toJson());
    return object;
    }

  bool JoinedRoomMessage::isValid() const
  {
    return true;
  }

  BaseMessage* JoinedRoomMessage::clone() const
  {
    return new JoinedRoomMessage(*this);
  }

  BaseMessage::MessageType JoinedRoomMessage::type() const
  {
    return TypeJoinedRoom;
  }

}

