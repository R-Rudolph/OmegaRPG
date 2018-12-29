#ifndef HEARTBEATMESSAGE_H
#define HEARTBEATMESSAGE_H

#include "abstractmessage.h"

namespace orpg
{
  /*
   * Heartbeat Message
   * Sent by clients to keep the alive the connection
   */
  class HeartbeatMessage : public AbstractMessage
  {
  public:
    static const QString messageType;
  public:
    HeartbeatMessage();
    HeartbeatMessage(const QJsonObject& object);

    virtual QJsonObject toJson() const;
    virtual bool isValid() const;
    virtual BaseMessage* clone() const;
    virtual MessageType type() const;
  };
}


#endif // HEARTBEATMESSAGE_H
