#ifndef ASSIGNROLEMESSAGE_H
#define ASSIGNROLEMESSAGE_H

#include "abstractmessage.h"
#include "core/multi/util/player.h"

namespace orpg
{
  /*
   * Assign Role Message
   * Sent by the client to change a player's role
   */
  class AssignRoleMessage : public AbstractMessage
  {
  public:
    static const QString messageType;
    static const QString idField;
    static const QString roleField;
    static const QString dmpassField;
  protected:
    QString _id;
    PlayerRole _role;
    QString _dmpass;
  public:
    AssignRoleMessage(const QString& playerId, const PlayerRole& role, const QString& dmpass=QString());
    AssignRoleMessage(const QJsonObject& object);

    virtual QJsonObject toJson() const;
    virtual bool isValid() const;
    virtual BaseMessage* clone() const;
    virtual MessageType type() const;

    QString id() const;
    void setId(const QString& id);
    PlayerRole role() const;
    void setRole(const PlayerRole& role);
    QString dmpass() const;
    void setDmpass(const QString& dmpass);
  };
}


#endif // ASSIGNROLEMESSAGE_H
