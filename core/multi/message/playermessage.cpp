#include "playermessage.h"

namespace orpg
{
  const QString PlayerMessage::messageType = "player";
  const QString PlayerMessage::idField = "id";
  const QString PlayerMessage::nameField = "name";
  const QString PlayerMessage::statusField = "status";
  const QString PlayerMessage::roleField = "role";

  QString PlayerMessage::id() const
  {
    return _id;
  }

  void PlayerMessage::setId(const QString& id)
  {
    _id = id;
  }

  QString PlayerMessage::name() const
  {
    return _name;
  }

  void PlayerMessage::setName(const QString& name)
  {
    _name = name;
  }

  PlayerRole PlayerMessage::role() const
  {
    return _role;
  }

  void PlayerMessage::setRole(const PlayerRole& role)
  {
    _role = role;
  }

  QString PlayerMessage::status() const
  {
    return _status;
  }

  void PlayerMessage::setStatus(const QString& status)
  {
    _status = status;
  }

  PlayerMessage::PlayerMessage(const QString& name, const QString& status, const QString& id, const PlayerRole& role) : AbstractMessage()
  {
    this->_id = id;
    this->_name = name;
    this->_status = status;
    this->_role = role;
  }

  PlayerMessage::PlayerMessage() : AbstractMessage()
  {
    this->_role = ROLE_NOINTRODUCTION;
  }

  PlayerMessage::PlayerMessage(const QJsonObject& object) : AbstractMessage()
  {
    this->_id = object[idField].toString();
    this->_name = object[nameField].toString();
    this->_status = object[statusField].toString();
    this->_role = (PlayerRole) object[roleField].toInt((int)ROLE_NOINTRODUCTION);
  }

  QJsonObject PlayerMessage::toJson() const
  {
    QJsonObject object;
    addToJson(object, actionField, messageType);
    addToJson(object,idField,_id);
    addToJson(object,nameField,_name);
    addToJson(object,statusField,_status);
    addToJson(object,roleField,(int)_role);
    return object;
  }

  bool PlayerMessage::isValid() const
  {
    return true;
  }

  BaseMessage* PlayerMessage::clone() const
  {
    return new PlayerMessage(*this);
  }

  BaseMessage::MessageType PlayerMessage::type() const
  {
    return TypePlayer;
  }
}

