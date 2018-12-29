#ifndef OPENRPGLOGGER_H
#define OPENRPGLOGGER_H

#include "core/multi/message/message.h"
#include "core/client/settings.h"
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QTextStream>
#include <QMap>

namespace orpg
{

  class OpenRpgLogger
  {
    QMap<QString,QString> idPlayerMap;
    QFile file;
    QTextStream ts;

    QString escapeString(QString text);

    void writeLineBeginning();
    void writeLineEnding();

    void digestMessage_chat(const ChatMessage& message);
    void digestMessage_player(const PlayerMessage& message);
    void digestMessage_joinedRoom(const JoinedRoomMessage& message);
    void digestMessage_removePlayer(const PlayerRemoveMessage& message);

    bool openFile();
  public:
    OpenRpgLogger();
    void connect(const QString addr);
    void connected();
    void disconnected();
    void digestMessage(const Message& msg);
  };

}

#endif // OPENRPGLOGGER_H
