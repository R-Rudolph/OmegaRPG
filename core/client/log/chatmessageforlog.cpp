#include "chatmessageforlog.h"

namespace orpg
{

  QMap<QString, QString> ChatMessageForLog::getAttributes(QXmlStreamReader &reader)
  {
    QMap<QString,QString> result;
    foreach(QXmlStreamAttribute attribute, reader.attributes())
    {
      result.insert(attribute.name().toString(),attribute.value().toString());
    }
    return result;
  }

  void ChatMessageForLog::readXml(QXmlStreamReader &reader)
  {
    while(!reader.atEnd())
    {
      reader.readNext();
      switch(reader.tokenType())
      {
        case QXmlStreamReader::StartElement:
        {
          if(reader.name()=="span")
          {
            QMap<QString,QString> attributes = getAttributes(reader);
            QString elementClass = attributes["class"];
            if(elementClass == "name")
            {
              playerName = reader.readElementText(QXmlStreamReader::IncludeChildElements);
            }
            else if(elementClass == "time")
            {
              time = reader.readElementText(QXmlStreamReader::IncludeChildElements);
            }
            else if(elementClass == "text")
            {
              color = attributes["data-color"];
              text = reader.readElementText(QXmlStreamReader::IncludeChildElements);
            }
            else
              readXml(reader);
          }
          else
            readXml(reader);
          break;
        }
        case QXmlStreamReader::NoToken:
        case QXmlStreamReader::Invalid:
        case QXmlStreamReader::StartDocument:
        case QXmlStreamReader::EndDocument:
        case QXmlStreamReader::EndElement:
          return;
        case QXmlStreamReader::Characters:
        case QXmlStreamReader::Comment:
        case QXmlStreamReader::DTD:
        case QXmlStreamReader::EntityReference:
        case QXmlStreamReader::ProcessingInstruction:
          break;
      }
    }
  }

  const QString& ChatMessageForLog::getText() const
  {
    return text;
  }

  const QString& ChatMessageForLog::getPlayerName() const
  {
      return playerName;
  }
  
  const QString& ChatMessageForLog::getTo() const
  {
    return to;
  }

  QString ChatMessageForLog::toHtml() const
  {
    return QString("<div>[%1] <strong>(%2) %3:</strong> <span style=\"color:%4;\">%5</span></div>").arg(getTime(),QString::number(playerId),playerName,getColor(),text);
  }

  QString ChatMessageForLog::getChannel() const
  {
    return channel;
  }

  QString ChatMessageForLog::getColor() const
  {
    return color;
  }

  QString ChatMessageForLog::getTime() const
  {
    return time;
  }

  void ChatMessageForLog::init(QXmlStreamReader &reader, const QMap<QString, QString> &attributes)
  {
    channel = attributes["data-channel"];
    to = attributes["data-to"];
    chatId = attributes["data-cid"].toInt();
    playerId = attributes["data-from"].toInt();
    readXml(reader);
  }

  ChatMessageForLog::ChatMessageForLog(QXmlStreamReader &reader)
  {
    QMap<QString,QString> attributes = getAttributes(reader);
    init(reader,attributes);
  }

  ChatMessageForLog::ChatMessageForLog(QXmlStreamReader &reader, const QMap<QString, QString>& attributes)
  {
    init(reader,attributes);
  }
}
