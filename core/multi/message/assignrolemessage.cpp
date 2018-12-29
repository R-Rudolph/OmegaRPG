#include "assignrolemessage.h"

namespace orpg
{
  const QString AssignRoleMessage::messageType = "assignStatus";
  const QString AssignRoleMessage::idField = "pid";
  const QString AssignRoleMessage::roleField = "status";
  const QString AssignRoleMessage::dmpassField = "dmpass";

  QString AssignRoleMessage::id() const
  {
    return _id;
  }

  void AssignRoleMessage::setId(const QString& id)
  {
    _id = id;
  }

  PlayerRole AssignRoleMessage::role() const
  {
    return _role;
  }

  void AssignRoleMessage::setRole(const PlayerRole& role)
  {
    _role = role;
  }

  QString AssignRoleMessage::dmpass() const
  {
    return _dmpass;
  }

  void AssignRoleMessage::setDmpass(const QString& dmpass)
  {
    _dmpass = dmpass;
  }

  AssignRoleMessage::AssignRoleMessage(const QString& playerId, const PlayerRole& role, const QString& dmpass) : AbstractMessage()
  {
    this->_id = playerId;
    this->_role = role;
    this->_dmpass = dmpass;
  }

  AssignRoleMessage::AssignRoleMessage(const QJsonObject& object) : AbstractMessage()
  {
    this->_id = object[idField].toString();
    this->_role = (PlayerRole) object[roleField].toInt((int)ROLE_NOTSET);
    this->_dmpass = object[dmpassField].toString();
  }

  QJsonObject AssignRoleMessage::toJson() const
  {
    QJsonObject object;
    addToJson(object, actionField, messageType);
    addToJson(object, idField, _id);
    addToJson(object, roleField, (int)_role);
    addToJson(object, dmpassField, _dmpass);
    return object;
  }

  bool AssignRoleMessage::isValid() const
  {
    return !_id.isEmpty();
  }

  BaseMessage* AssignRoleMessage::clone() const
  {
    return new AssignRoleMessage(*this);
  }

  BaseMessage::MessageType AssignRoleMessage::type() const
  {
    return TypeAssignRole;
  }

}
