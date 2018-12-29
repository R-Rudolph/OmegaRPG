#ifndef JSONMESSAGELOGGER_H
#define JSONMESSAGELOGGER_H

#include "core/client/log/messagelogger.h"
#include<QJsonValue>
#include<QFile>
#include<QMap>
#include "core/client/log/elements/baselogelement.h"

namespace orpg
{
  class JsonMessageLogger : public MessageLogger
  {
    Q_OBJECT
    QMap<QString,QString> idNameMap;
    QMap<QString,QString> roomUidMap;
    QString currentUid;
  protected:
    QFile file;
    void writeJson(const QJsonObject& value);
    void write(const BaseLogElement& element);
    virtual void digestMessage_chat(const ChatMessage& msg);
    virtual void digestMessage_player(const PlayerMessage& msg);
    virtual void digestMessage_removePlayer(const PlayerRemoveMessage& msg);
    virtual void digestMessage_joinedRoom(const JoinedRoomMessage& msg);
    virtual void messageDigestStart(const Message& msg);
    virtual void messageDigestFinish(const Message& msg);
    virtual bool initSession(const JoinedRoomMessage& msg);
    virtual void finishSession();
    QString fileDateString();
    bool sessionRunning();
  public:
    JsonMessageLogger(QObject* parent=nullptr);
    ~JsonMessageLogger();
  public slots:
    virtual void disconnected();
  };
}


#endif // JSONMESSAGELOGGER_H
