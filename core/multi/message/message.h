#ifndef MESSAGE_H
#define MESSAGE_H

#include "core/multi/message/basemessage.h"

#include "core/multi/message/assignrolemessage.h"
#include "core/multi/message/chatmessage.h"
#include "core/multi/message/createroommessage.h"
#include "core/multi/message/givestatusmessage.h"
#include "core/multi/message/heartbeatmessage.h"
#include "core/multi/message/joinedroommessage.h"
#include "core/multi/message/joinroommessage.h"
#include "core/multi/message/kickmessage.h"
#include "core/multi/message/playermessage.h"
#include "core/multi/message/playerremovemessage.h"
#include "core/multi/message/roomresourcemessage.h"
#include "core/multi/message/roomvariablemessage.h"
#include "core/multi/message/setidmessage.h"
#include "core/multi/message/roomsmessage.h"
#include "core/multi/message/unknownmessage.h"

namespace orpg
{
  class Message : public BaseMessage
  {
  public:
    static const QMap<QString,MessageType>& stringTypeMap();
  private:
    static QMap<QString,MessageType> _stringTypeMap;
  protected:
    BaseMessage* _wrappedMessage;
  public:
    Message();
    Message(const BaseMessage& message);
    Message(const Message& other);
    Message(const QJsonObject& object);
    Message(const QByteArray& byteArray);
    ~Message();


    virtual QJsonObject toJson() const;
    virtual bool isValid() const;
    virtual QDateTime time() const;
    virtual BaseMessage* clone() const;
    virtual MessageType type() const;
    virtual QString room() const;
    virtual void setRoom(const QString& room);
    virtual QString senderId() const;
    virtual void setSenderId(const QString& senderId);
    virtual PlayerRole senderRole() const;
    virtual void setSenderRole(const PlayerRole& senderRole);

    template<class T> T toMessage() const
    {
      return T(* static_cast<T*>(_wrappedMessage));
    }

  };

}

#endif // MESSAGE_H
