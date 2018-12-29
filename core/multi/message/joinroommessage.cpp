#include "joinroommessage.h"

namespace orpg
{
  const QString JoinRoomMessage::messageType = "joinRoom";
  const QString JoinRoomMessage::roomIdField = "rid";
  const QString JoinRoomMessage::passwordField = "pass";

  int JoinRoomMessage::roomId() const
  {
    return _roomId;
  }

  void JoinRoomMessage::setRoomId(int roomId)
  {
    _roomId = roomId;
  }

  QString JoinRoomMessage::password() const
  {
    return _password;
  }

  void JoinRoomMessage::setPassword(const QString& password)
  {
    _password = password;
  }

  JoinRoomMessage::JoinRoomMessage(int roomId, const QString& password) : AbstractMessage()
  {
    _roomId = roomId;
    _password = password;
  }

  JoinRoomMessage::JoinRoomMessage(const QJsonObject& object) : AbstractMessage()
  {
    _roomId = object[roomIdField].toInt(-1);
    if(_roomId == -1)
    {
      bool ok;
      _roomId = object[roomIdField].toString().toInt(&ok);
      if(!ok)
        _roomId = -1;
    }
    _password = object[passwordField].toString();
  }

  QJsonObject JoinRoomMessage::toJson() const
  {
    QJsonObject object;
    addToJson(object,actionField,messageType);
    addToJson(object,roomIdField,_roomId);
    addToJson(object,passwordField,_password);
    return object;
  }

  bool JoinRoomMessage::isValid() const
  {
    return _roomId >= 0;
  }

  BaseMessage* JoinRoomMessage::clone() const
  {
    return new JoinRoomMessage(*this);
  }

  BaseMessage::MessageType JoinRoomMessage::type() const
  {
    return TypeJoinRoom;
  }
}
