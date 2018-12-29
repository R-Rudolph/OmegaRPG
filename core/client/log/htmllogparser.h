#ifndef HTMLLOGPARSER_H
#define HTMLLOGPARSER_H

#include<QMap>
#include<QList>
#include<QString>
#include<QDateTime>
#include<QDomElement>

#include "core/client/log/elements/logelement.h"
#include "core/client/log/elements/logchatelement.h"
#include "core/client/log/elements/logplayerelement.h"
#include "core/client/log/elements/loginfoelement.h"
#include "core/client/log/elements/logplayerremoveelement.h"

#include "logparser.h"

namespace orpg
{
  class HtmlLogParser : public LogParser
  {
  private:
    enum HtmlLogElements
    {
      UnknownElement,
      Server,
      Room,
      Time,
      Chat,
      Player,
      PlayerRemove
    };
    static const QMap<QString,HtmlLogElements> elementClassMap;
    QString errorString;
    //internal variables
    QDateTime previousTime;
    QString determinedViewerId;
    //parsing functions
    QDateTime getTime(const QDomElement& messageElement);
    LogChatElement getLogChatElement(const QDomElement& element);
    LogPlayerElement getLogPlayerElement(const QDomElement& element);
    LogPlayerRemoveElement getLogPlayerRemoveElement(const QDomElement& element);
    void parse(const QByteArray& data);

    HtmlLogParser();
  public:
    HtmlLogParser(const QString& filename);
    HtmlLogParser(const QByteArray& data);
    QString getFilename() const;
  };
}

#endif // HTMLLOGPARSER_H
