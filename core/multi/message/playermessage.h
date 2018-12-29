#ifndef PLAYERMESSAGE_H
#define PLAYERMESSAGE_H

#include "abstractmessage.h"
#include "core/multi/util/player.h"

namespace orpg
{
  /*
   * Player Message
   * Sent by clients to change their player name and status
   * Sent by servers to update client's player lists
   */
  class PlayerMessage : public AbstractMessage
  {
  public:
    static const QString messageType;
    static const QString idField;
    static const QString nameField;
    static const QString roleField;
    static const QString statusField;
  protected:
    QString _id;
    QString _name;
    PlayerRole _role;
    QString _status;
  public:
    PlayerMessage();
    PlayerMessage(const QString& name, const QString& status, const QString& id=QString(), const PlayerRole& role=ROLE_NOINTRODUCTION);
    PlayerMessage(const QJsonObject& object);

    virtual QJsonObject toJson() const;
    virtual bool isValid() const;
    virtual BaseMessage* clone() const;
    virtual MessageType type() const;

    QString id() const;
    void setId(const QString& id);
    QString name() const;
    void setName(const QString& name);
    PlayerRole role() const;
    void setRole(const PlayerRole& role);
    QString status() const;
    void setStatus(const QString& status);
  };
}


#endif // PLAYERMESSAGE_H
