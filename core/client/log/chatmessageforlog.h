#ifndef CHATMESSAGEFORLOG_H
#define CHATMESSAGEFORLOG_H

#include <QXmlStreamReader>
#include <QMap>
#include <QString>


namespace orpg
{
  class ChatMessageForLog
  {
    //fields
    QString playerName;
    QString channel;
    int playerId;
    int chatId;
    QString to;
    QString text;
    QString color;
    QString time;
    //helper functions
    void init(QXmlStreamReader& reader, const QMap<QString,QString>& attributes);
    QMap<QString,QString> getAttributes(QXmlStreamReader &reader);
    void readXml(QXmlStreamReader& reader);
  public:
    ChatMessageForLog(QXmlStreamReader& reader);
    ChatMessageForLog(QXmlStreamReader& reader, const QMap<QString,QString>& attributes);
    const QString& getText() const;
    const QString& getPlayerName() const;
    const QString& getTo() const;
    QString toHtml() const;
    QString getChannel() const;
    QString getColor() const;
    QString getTime() const;
  };
}

#endif // CHATMESSAGEFORLOG_H
