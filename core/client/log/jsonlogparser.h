#ifndef JSONMESSAGEPARSER_H
#define JSONMESSAGEPARSER_H

#include<QJsonParseError>
#include<QVector>
#include<QDateTime>
#include<QString>
#include<QSet>
#include<QMap>
#include<QPair>
#include "elements/loginfoelement.h"
#include "elements/logelement.h"

#include "logparser.h"

class QByteArray;

namespace orpg
{
  class JsonLogParser : public LogParser
  {
    static bool registered;
  protected:
    QString filename;
    QJsonParseError jsonParseError;

    void parse(const QByteArray& data);
  public:
    JsonLogParser();
    virtual ~JsonLogParser();
    JsonLogParser(const QString& filename);
    JsonLogParser(const QByteArray& data);
    QString getFilename() const;
    static void registerTypes();
  };
}


#endif // JSONMESSAGEPARSER_H
