#ifndef SessionMessageLogger2_H
#define SessionMessageLogger2_H

#include <QDateTime>
#include <QFile>
#include <QString>
#include <QMap>
#include <QTextStream>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <utility>
#include "core/multi/message/message.h"
#include "core/multi/util/global.h"
#include "core/client/settings.h"

namespace orpg
{
  class SessionMessageLogger2
  {
    QMap<QString,QString> playerMap;

    QFile file;
    QXmlStreamWriter xml;

    QString serverName;
    QDateTime dtBegin;
    QString roomName;

    QString currentFilename;
    QString currentId;
    bool chatMessageReceived;
    bool firstRoomEntered;

    QString getChannel(const QString from, const QString& to);

    bool init();
    void writePlayer(const QString& text, const QString& name, const QString& id);
    void writeHtml(QString text);
    void parse(QXmlStreamReader& reader, bool openedTag);

    void digestMessage_chat(const ChatMessage& msg);
    void digestMessage_player(const PlayerMessage& msg);
    void digestMessage_removePlayer(const PlayerRemoveMessage& msg);
    void digestMessage_joinedRoom(const JoinedRoomMessage& msg);
    void digestMessage_setId(const SetIdMessage& msg);
    void cleanup();
  public:
    SessionMessageLogger2();
    ~SessionMessageLogger2();
    void setServer(const QString& serverName);
    void digestMessage(const Message& msg);
  };

}

#endif // SessionMessageLogger2_H
