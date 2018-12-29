#include "sessionlogparser.h"

namespace orpg
{
  const QString SessionLogParser::mainChannelString = "Main Channel";

  SessionLogParser::SessionLogParser(const QString &filepath)
  {
    serverFound = false;
    roomFound = false;
    timeFound = false;
    parsed = false;
    this->filepath = filepath;
    parse(true);
  }

  SessionLogParser::SessionLogParser(const QByteArray &data)
  {
    serverFound = false;
    roomFound = false;
    timeFound = false;
    parsed = false;
    parse(data);
  }

  bool SessionLogParser::isValid()
  {
    return serverFound && roomFound && timeFound;
  }

  QString SessionLogParser::getServer() const
  {
    return server;
  }

  QString SessionLogParser::getRoom() const
  {
    return room;
  }

  QDateTime SessionLogParser::getTime() const
  {
    return time;
  }

  const QList<ChatMessageForLog> &SessionLogParser::getMessages()
  {
    if(!parsed)
      parse();
    return messages;
  }

  QString SessionLogParser::getFilename() const
  {
    return filename;
  }

  QString SessionLogParser::getChannelName(const QString& channel)
  {
    if(idPlayerMap.contains(channel))
    {
      return QString("(%1) %2").arg(channel,idPlayerMap.value(channel));
    }
    else
    {
      return "Main Channel";
    }
  }

  bool SessionLogParser::parse(QXmlStreamReader &reader, bool headerOnly)
  {
    serverFound = false;
    roomFound = false;
    timeFound = false;
    if(!skipToContent(reader))
      return false;
    while(reader.readNextStartElement())
    {
      if(reader.name()=="div")
      {
        QMap<QString,QString> attributes = getAttributes(reader);
        QString classAttribute = attributes["class"];
        if(classAttribute == "room")
        {
          roomFound = true;
          room = attributes["data-room"];
          reader.skipCurrentElement();
        }
        else if(classAttribute == "time")
        {
          timeFound = true;
          time = QDateTime::fromString(attributes["data-time"],"yyyy-MM-dd hh:mm:ss");
          mostPreviousTime = time;
          reader.skipCurrentElement();
        }
        else if(classAttribute == "server")
        {
          serverFound = true;
          server = attributes["data-server"];
          reader.skipCurrentElement();
        }
        else if(classAttribute == "player")
        {
          idPlayerMap.insert(attributes["data-id"],attributes["data-name"]);
          reader.skipCurrentElement();
        }
        else if(classAttribute == "chat")
        {
          ChatMessageForLog msg(reader,attributes);
          messages.push_back(msg);
        }
        else
          reader.skipCurrentElement();
      }
      else
        reader.skipCurrentElement();
      if(headerOnly && serverFound && roomFound && timeFound)
      {
        return true;
      }
    }
    return serverFound && roomFound && timeFound;
  }

  bool SessionLogParser::parse(const QByteArray &data, bool headerOnly)
  {
    QXmlStreamReader reader(data);
    return parse(reader,headerOnly);
  }

  void SessionLogParser::free()
  {
    messages.clear();
  }

  bool SessionLogParser::skipToContent(QXmlStreamReader &reader)
  {
    while(!reader.atEnd())
    {
      reader.readNext();
      switch(reader.tokenType())
      {
        case QXmlStreamReader::StartElement:
        {
          if(reader.name()=="html")
            break;
          else if(reader.name()=="head")
          {
            reader.skipCurrentElement();
            break;
          }
          else if(reader.name()=="body")
            return true;
          else
            return false;
        }
        case QXmlStreamReader::Invalid:
        case QXmlStreamReader::NoToken:
        case QXmlStreamReader::EndDocument:
        case QXmlStreamReader::EndElement:
          return false;
        default:
          break;
      }
    }
    return false;
  }

  QMap<QString, QString> SessionLogParser::getAttributes(QXmlStreamReader &reader)
  {
    QMap<QString,QString> result;
    foreach(QXmlStreamAttribute attribute, reader.attributes())
    {
      result.insert(attribute.name().toString(),attribute.value().toString());
    }
    return result;
  }

  bool SessionLogParser::parse(bool headerOnly)
  {
    if(parsed)
      return true;
    if(filepath.isEmpty())
      return false;
    QFile file(filepath);
    filename = file.fileName();
    if(file.open(QIODevice::ReadOnly))
    {
      QXmlStreamReader reader(&file);
      if(headerOnly)
      {
        return parse(reader,headerOnly);
      }
      else
      {
        parsed = parse(reader,headerOnly);
        return parsed;
      }
    }
    else
      return false;
  }
}
