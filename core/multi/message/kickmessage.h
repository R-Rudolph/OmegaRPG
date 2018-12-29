#ifndef KICKMESSAGE_H
#define KICKMESSAGE_H

#include "abstractmessage.h"

namespace orpg
{
  /*
   * Kick Message
   * Sent by clients to ask the server to remove a player from the room
   */
  class KickMessage : public AbstractMessage
  {
  public:
    static const QString messageType;
    static const QString idField;
  protected:
    QString _id;
  public:
    KickMessage(const QJsonObject& object);
    KickMessage(const QString& id);
    KickMessage(quint32 id);

    virtual QJsonObject toJson() const;
    virtual bool isValid() const;
    virtual BaseMessage* clone() const;
    virtual MessageType type() const;

    QString id() const;
    void setId(const QString& id);
  };
}


#endif // KICKMESSAGE_H
