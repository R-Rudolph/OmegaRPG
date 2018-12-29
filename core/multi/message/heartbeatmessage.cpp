#include "heartbeatmessage.h"

namespace orpg
{
  const QString HeartbeatMessage::messageType = "can't stop the signal";

  HeartbeatMessage::HeartbeatMessage() : AbstractMessage()
  {

  }

  HeartbeatMessage::HeartbeatMessage(const QJsonObject&) : HeartbeatMessage()
  {

  }

  QJsonObject HeartbeatMessage::toJson() const
  {
    QJsonObject object;
    addToJson(object, actionField, messageType);
    return object;
  }

  bool HeartbeatMessage::isValid() const
  {
    //heartbeats are always super valid.
    return true;
  }

  BaseMessage*HeartbeatMessage::clone() const
  {
    return new HeartbeatMessage(*this);
  }

  BaseMessage::MessageType HeartbeatMessage::type() const
  {
    return TypeHeartbeat;
  }
}

