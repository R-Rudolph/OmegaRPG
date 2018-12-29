#ifndef ROOMSMESSAGE_H
#define ROOMSMESSAGE_H

#include "abstractmessage.h"
#include "core/multi/util/roominfo.h"

namespace orpg
{
  /*
   * Rooms Message
   * Sent by servers to notify a client of the current rooms
   */
  class RoomsMessage : public AbstractMessage
  {
  public:
    static const QString messageType;
    static const QString roomsField;
  protected:
    QList<RoomInfo> _rooms;
  public:
    RoomsMessage(const QList<RoomInfo>& rooms);
    RoomsMessage(const QJsonObject& object);

    virtual QJsonObject toJson() const;
    virtual bool isValid() const;
    virtual BaseMessage* clone() const;
    virtual MessageType type() const;

    QList<RoomInfo> rooms() const;
    void setRooms(const QList<RoomInfo>& rooms);
  };
}


#endif // ROOMSMESSAGE_H
