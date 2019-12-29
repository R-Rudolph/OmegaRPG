#ifndef LOGPARSER_H
#define LOGPARSER_H

#include<QString>
#include<QMultiMap>
#include<QVector>

#include "elements/loginfoelement.h"
#include "elements/logelement.h"

#include "ChannelName.h"

namespace orpg
{
  class LogParser
  {
  public:
    static const QString allString;
    static const QString gmString;
    static const QString mixedString;
    static const QString mainChannelString;
    static const QString gmChannelString;
    static const QString mixedChannelString;
    enum Error
    {
      Unknown,
      NoError,
      FileOpenError,
      FileFormatError,
      MalformedLogfileError
    };
  protected:

    //content info
    QMultiMap<QString,QString> idNameMap;
    QVector<LogElement> elements;
    LogInfoElement info;
    int numberChatMessages;
    //output settings
    QString infoColor;
    //error data
    Error error;

    //utility functions
    void appendToMap(const ChannelName& key, const QString& value, QMap<ChannelName,QString>& map);
    QString getName(const QString& id);

    //html output functions
    QString chatTextHtml(const LogChatElement& element, bool explicitTo=false);
    QString infoTextHtml(const QDateTime& time, const QString& text);
    QMap<ChannelName,QString> getChannels();

    LogParser();
  public:
    virtual ~LogParser();
    QList< QPair<QString,QString> > getHtmlLogByChannels();
    QString toHtmlWithChannels_Experimental();
    QJsonArray toJson() const;
    virtual bool isValid();
    QString getRoomName() const;
    QString getServerName() const;
    QDateTime getTime() const;
    const QVector<LogElement>& getElements();
    QString getUID() const;
  };
}

#endif // LOGPARSER_H
