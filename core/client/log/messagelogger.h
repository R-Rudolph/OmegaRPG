#ifndef MESSAGELOGGER_H
#define MESSAGELOGGER_H

#include<QString>
#include<QObject>
#include "core/multi/message/message.h"

namespace orpg
{
  class MessageLogger : public QObject
  {
    Q_OBJECT
  protected:
    static const QSet<BaseMessage::MessageType> handledMessageTypes;

    int chatMessageCount;
    QString currentServername;
    QString currentId;

    virtual void digestMessage_chat(const ChatMessage& msg) = 0;
    virtual void digestMessage_player(const PlayerMessage& msg) = 0;
    virtual void digestMessage_removePlayer(const PlayerRemoveMessage& msg) = 0;
    virtual void digestMessage_joinedRoom(const JoinedRoomMessage& msg) = 0;
    virtual void digestMessage_setID(const SetIdMessage& msg);
    virtual void messageDigestStart(const Message& msg);
    virtual void messageDigestFinish(const Message& msg);
    virtual bool initSession(const JoinedRoomMessage& msg) = 0;
    virtual void finishSession() = 0;
  public:
    MessageLogger(QObject* parent=nullptr);
    void setServer(const QString& value);

  public slots:
    void digestMessage(const Message& msg);
    virtual void disconnected() = 0;
  signals:
    void error();
  };
}

#endif // MESSAGELOGGER_H
