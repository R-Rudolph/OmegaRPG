#ifndef PLAYERREMOVEMESSAGE_H
#define PLAYERREMOVEMESSAGE_H

#include "abstractmessage.h"

namespace orpg
{
  /*
   * Player Remove Message
   * Sent by servers to signal a player leaving a room
   */
  class PlayerRemoveMessage : public AbstractMessage
  {
  public:
    static const QString messageType;
    static const QString idField;
  protected:
    QString _id;
  public:
    PlayerRemoveMessage();
    PlayerRemoveMessage(const QString& id);
    PlayerRemoveMessage(const QJsonObject& object);

    virtual QJsonObject toJson() const;
    virtual bool isValid() const;
    virtual BaseMessage* clone() const;
    virtual MessageType type() const;

    QString id() const;
    void setId(const QString& id);
  };
}


#endif // PLAYERREMOVEMESSAGE_H
