#include "playerremovemessage.h"

namespace orpg
{
  const QString PlayerRemoveMessage::messageType = "player_remove";
  const QString PlayerRemoveMessage::idField = "id";

  PlayerRemoveMessage::PlayerRemoveMessage() : AbstractMessage()
  {

  }

  PlayerRemoveMessage::PlayerRemoveMessage(const QString& id) : AbstractMessage()
  {
    this->_id = id;
  }

  PlayerRemoveMessage::PlayerRemoveMessage(const QJsonObject& object) : AbstractMessage()
  {
    this->_id = object[idField].toString();
  }

  QJsonObject PlayerRemoveMessage::toJson() const
  {
    QJsonObject object;
    addToJson(object, actionField, messageType);
    addToJson(object,idField,_id);
    return object;
  }

  bool PlayerRemoveMessage::isValid() const
  {
    return !_id.isEmpty();
  }

  BaseMessage* PlayerRemoveMessage::clone() const
  {
    return new PlayerRemoveMessage(*this);
  }

  BaseMessage::MessageType PlayerRemoveMessage::type() const
  {
    return TypePlayerRemove;
  }

  QString PlayerRemoveMessage::id() const
  {
    return _id;
  }

  void PlayerRemoveMessage::setId(const QString& id)
  {
    _id = id;
  }

}
