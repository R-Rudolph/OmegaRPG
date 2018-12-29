#ifndef GIVESTATUSMESSAGE_H
#define GIVESTATUSMESSAGE_H

#include "abstractmessage.h"
#include "core/multi/util/player.h"

namespace orpg
{
  /*
   * Create Room Message
   * Sent by servers to notify a client of it's changed role
   */
  class GiveRoleMessage : public AbstractMessage
  {
  public:
    static const QString messageType;
    static const QString roleField;
  protected:
    PlayerRole _role;
  public:
    GiveRoleMessage(const PlayerRole& role);
    GiveRoleMessage(const QJsonObject& object);

    virtual QJsonObject toJson() const;
    virtual bool isValid() const;
    virtual BaseMessage* clone() const;
    virtual MessageType type() const;

    PlayerRole role() const;
  };
}


#endif // GIVESTATUSMESSAGE_H
