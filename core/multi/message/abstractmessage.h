#ifndef ABSTRACTMESSAGE_H
#define ABSTRACTMESSAGE_H

#include "basemessage.h"

namespace orpg
{
  class AbstractMessage : public BaseMessage
  {
  public:
    static const QString idField;
  protected:
    QString _room;
    QString _senderId;
    PlayerRole _senderRole;
    QDateTime _time;
    AbstractMessage();
    AbstractMessage(const QJsonObject& object);
  public:
    virtual ~AbstractMessage();
    virtual QDateTime time() const;
    virtual QString room() const;
    virtual void setRoom(const QString& room);
    virtual QString senderId() const;
    virtual void setSenderId(const QString& senderId);
    virtual bool hasSenderId() const;
    virtual PlayerRole senderRole() const;
    virtual void setSenderRole(const PlayerRole& senderRole);
    virtual QJsonObject toJson() const;
  };
}

#endif // ABSTRACTMESSAGE_H
