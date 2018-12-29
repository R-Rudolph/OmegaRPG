#ifndef SESSIONLOGPARSER_H
#define SESSIONLOGPARSER_H

#include <QObject>
#include <QFile>
#include <QString>
#include <QXmlStreamReader>
#include <QByteArray>
#include <QList>
#include <QMap>
#include <QDateTime>
#include <QMultiMap>

#include "core/client/log/chatmessageforlog.h"

namespace orpg
{
  class SessionLogParser
  {
    //log header
    QString server;
    QString room;
    QDateTime time;
    bool serverFound;
    bool roomFound;
    bool timeFound;
    //log messages
    QDateTime mostPreviousTime;
    QMultiMap<QString,QString> idPlayerMap;
    QList<ChatMessageForLog> messages;
    //given info
    QString filepath;
    QString filename;
    //status
    bool parsed;
    //parse functions
    bool parse(QXmlStreamReader& reader, bool headerOnly=false);
    bool parse(const QByteArray& data, bool headerOnly=false);
    //other
    void free();
    bool skipToContent(QXmlStreamReader& reader);
    QMap<QString,QString> getAttributes(QXmlStreamReader& reader);
  public:
    const static QString mainChannelString;
    explicit SessionLogParser(const QString& filepath);
    explicit SessionLogParser(const QByteArray& data);
    bool isValid();
    bool parse(bool headerOnly=false);
    QString getServer() const;
    QString getRoom() const;
    QDateTime getTime() const;
    const QList<ChatMessageForLog>& getMessages();
    QString getFilename() const;
    QString getChannelName(const QString& channel);
  };
}

#endif // SESSIONLOGPARSER_H
