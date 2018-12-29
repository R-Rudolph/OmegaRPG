#include "givestatusmessage.h"

namespace orpg
{
  const QString GiveRoleMessage::messageType = "giveStatus";
  const QString GiveRoleMessage::roleField = "status";

  PlayerRole GiveRoleMessage::role() const
  {
      return _role;
  }
  
  GiveRoleMessage::GiveRoleMessage(const PlayerRole& role) : AbstractMessage()
  {
    _role = role;
  }

  GiveRoleMessage::GiveRoleMessage(const QJsonObject& object) : AbstractMessage()
  {
    _role = (PlayerRole) object[roleField].toInt(ROLE_NOINTRODUCTION);
  }

  QJsonObject GiveRoleMessage::toJson() const
  {
    QJsonObject object;
    addToJson(object,actionField,messageType);
    addToJson(object,roleField,_role);
    return object;
  }

  bool GiveRoleMessage::isValid() const
  {
    return true;
  }

  BaseMessage* GiveRoleMessage::clone() const
  {
    return new GiveRoleMessage(*this);
  }

  BaseMessage::MessageType GiveRoleMessage::type() const
  {
    return TypeGiveRole;
  }
}
