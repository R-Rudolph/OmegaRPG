#include "abstractmessage.h"

namespace orpg
{
  const QString AbstractMessage::idField = "id";

  QString AbstractMessage::room() const
  {
      return _room;
  }

  void AbstractMessage::setRoom(const QString& room)
  {
      _room = room;
  }

  QString AbstractMessage::senderId() const
  {
      return _senderId;
  }

  void AbstractMessage::setSenderId(const QString& senderId)
  {
    _senderId = senderId;
  }

  bool AbstractMessage::hasSenderId() const
  {
    return !_senderId.isEmpty();
  }

  PlayerRole AbstractMessage::senderRole() const
  {
    return _senderRole;
  }

  void AbstractMessage::setSenderRole(const PlayerRole& senderRole)
  {
    _senderRole = senderRole;
  }

  QJsonObject AbstractMessage::toJson() const
  {
    QJsonObject object;
    addToJson(object,idField,_senderId);
    return object;
  }

  AbstractMessage::AbstractMessage()
  {
    _senderRole = ROLE_NOTSET;
    _time = QDateTime::currentDateTime();
  }

  AbstractMessage::AbstractMessage(const QJsonObject& object) : AbstractMessage()
  {
    _senderId = object[idField].toString();
  }

  AbstractMessage::~AbstractMessage()
  {

  }

  QDateTime AbstractMessage::time() const
  {
    return _time;
  }
}
