#ifndef JOINEDROOMMESSAGE_H
#define JOINEDROOMMESSAGE_H

#include "abstractmessage.h"
#include "core/multi/util/roominfo.h"

namespace orpg
{
  /*
   * Joined Room Message
   * Sent by servers to notify them of having joined a room
   */
  class JoinedRoomMessage : public AbstractMessage
  {
  public:
    static const QString messageType;
    static const QString roomField;
  protected:
    RoomInfo _roomInfo;
  public:
    JoinedRoomMessage(const RoomInfo& roomInfo);
    JoinedRoomMessage(const QJsonObject& object);

    virtual QJsonObject toJson() const;
    virtual bool isValid() const;
    virtual BaseMessage* clone() const;
    virtual MessageType type() const;

    RoomInfo roomInfo() const;
    void setRoomInfo(const RoomInfo& roomInfo);
  };
}


#endif // JOINEDROOMMESSAGE_H
