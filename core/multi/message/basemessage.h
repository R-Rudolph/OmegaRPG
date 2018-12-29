#ifndef BASEMESSAGE_H
#define BASEMESSAGE_H

#include<QJsonObject>
#include<QDateTime>
#include "core/multi/util/player.h"

namespace orpg
{
  class BaseMessage
  {
  public:
    enum MessageType
    {
      TypeNotSet,
      TypeAssignRole,
      TypeChatMessage,
      TypeCreateRoom,
      TypeGiveRole,
      TypeHeartbeat,
      TypeJoinedRoom,
      TypeJoinRoom,
      TypeKick,
      TypePlayer,
      TypePlayerRemove,
      TypeRooms,
      TypeSetId,
      TypeRoomVariable,
      TypeRoomResource,
      TypeUnknown
    };
    static const QString actionField;
  protected:
    BaseMessage();
    void addToJson(QJsonObject& object, const QString& field, const QJsonValue& value) const;
  public:
    virtual  ~BaseMessage();
    virtual QJsonObject toJson() const = 0;
    virtual bool isValid() const = 0;
    virtual QDateTime time() const = 0;
    virtual BaseMessage* clone() const = 0;
    virtual MessageType type() const = 0;
    virtual QByteArray toByteArray() const;

    virtual QString room() const = 0;
    virtual void setRoom(const QString& room) = 0;
    virtual QString senderId() const = 0;
    virtual void setSenderId(const QString& senderId) = 0;
    virtual PlayerRole senderRole() const = 0;
    virtual void setSenderRole(const PlayerRole& senderRole) = 0;
  };
}


#endif // BASEMESSAGE_H
