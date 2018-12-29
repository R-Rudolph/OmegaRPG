#ifndef JOINROOMMESSAGE_H
#define JOINROOMMESSAGE_H

#include "abstractmessage.h"

namespace orpg
{
  /*
   * Join Room Message
   * Sent by client to try to join a room
   */
  class JoinRoomMessage : public AbstractMessage
  {
  public:
    static const QString messageType;
    static const QString roomIdField;
    static const QString passwordField;
  protected:
    int _roomId;
    QString _password;
  public:
    JoinRoomMessage(int roomId, const QString& password=QString());
    JoinRoomMessage(const QJsonObject& object);

    virtual QJsonObject toJson() const;
    virtual bool isValid() const;
    virtual BaseMessage* clone() const;
    virtual MessageType type() const;

    int roomId() const;
    void setRoomId(int roomId);
    QString password() const;
    void setPassword(const QString& password);
  };
}


#endif // JOINROOMMESSAGE_H
