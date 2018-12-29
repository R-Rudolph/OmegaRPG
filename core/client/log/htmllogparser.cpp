#include "htmllogparser.h"

#include<QFile>
#include<QDomDocument>
#include<QTextStream>

#include "core/client/log/elements/logchatelement.h"
#include "core/client/log/elements/loginfoelement.h"

namespace orpg
{

  const QMap<QString,HtmlLogParser::HtmlLogElements> HtmlLogParser::elementClassMap({
      std::pair<QString,HtmlLogParser::HtmlLogElements>("server",HtmlLogParser::Server),
      std::pair<QString,HtmlLogParser::HtmlLogElements>("room",HtmlLogParser::Room),
      std::pair<QString,HtmlLogParser::HtmlLogElements>("time",HtmlLogParser::Time),
      std::pair<QString,HtmlLogParser::HtmlLogElements>("chat",HtmlLogParser::Chat),
      std::pair<QString,HtmlLogParser::HtmlLogElements>("player",HtmlLogParser::Player),
      std::pair<QString,HtmlLogParser::HtmlLogElements>("player_remove",HtmlLogParser::PlayerRemove)
      });

  QDateTime HtmlLogParser::getTime(const QDomElement& messageElement)
  {
    QString timeString;
    QDomElement spanElement = messageElement.firstChildElement("span");
    while(!spanElement.isNull())
    {
      if(spanElement.attribute("class")=="time")
      {
        timeString = spanElement.text();
      }
      spanElement = spanElement.nextSiblingElement("span");
    }
    if(!timeString.isEmpty())
    {
      QDateTime time = QDateTime::fromString(timeString,"hh:mm");
      time.setDate(previousTime.date());
      //check for date change
      if(time<previousTime)
      {
        if(time.addSecs(60)>=previousTime)
        {
          time = previousTime;
        }
        else
        {
          time = time.addDays(1);
        }
      }
      previousTime = time;
      return time;
    }
    else
    {
      return QDateTime();
    }
  }

  LogChatElement HtmlLogParser::getLogChatElement(const QDomElement& element)
  {
    QDateTime time = getTime(element);
    QString from = element.attribute("data-from");
    QString to = element.attribute("data-to");
    if(determinedViewerId.isEmpty())
    {
      QString channel = element.attribute("data-channel");
      if(channel!="all" && channel!="dm" && !channel.isEmpty())
      {
        if(channel==from)
          determinedViewerId = from;
        else if(channel==to)
          determinedViewerId = to;
      }
    }
    QString name = element.firstChildElement("strong").firstChildElement("span").text();
    QDomElement textElement = element.lastChildElement("span");
    QString text;
    QString color = textElement.attribute("data-color");
    QTextStream ts(&text);
    {
      QDomNode node = textElement.firstChild();
      while(!node.isNull())
      {
        ts << node;
        node = node.nextSibling();
      }
    }
    return LogChatElement(from,to,name,color,text,time);
  }

  LogPlayerElement HtmlLogParser::getLogPlayerElement(const QDomElement& element)
  {
    QDateTime time = getTime(element);
    QString id = element.attribute("data-id");
    QString name = element.attribute("data-name");
    return LogPlayerElement(id,name,time);
  }

  LogPlayerRemoveElement HtmlLogParser::getLogPlayerRemoveElement(const QDomElement& element)
  {
    QDateTime time = getTime(element);
    QString id = element.attribute("data-id");
    return LogPlayerRemoveElement(id,time);
  }

  void HtmlLogParser::parse(const QByteArray& data)
  {
    QDomDocument doc("log");
    if(!doc.setContent(data,false,&errorString))
    {
      QByteArray data2 = data + QString("</body></html>").toUtf8();
      if(!doc.setContent(data2,false))
      {
        error = FileFormatError;
        return;
      }
    }
    QDomElement html = doc.documentElement();
    QDomElement body = html.firstChildElement("body");
    QDomElement message = body.firstChildElement("div");

    QString server;
    QString room;
    QDateTime time;
    while(!message.isNull())
    {
      QString elementClass = message.attribute("class");
      switch(elementClassMap[elementClass])
      {
        case Server:
          server = message.attribute("data-server");
          break;
        case Room:
          room = message.attribute("data-room");
          break;
        case Time:
        {
          QDateTime t = QDateTime::fromString(message.attribute("data-time"),"yyyy-MM-dd hh:mm:ss");
          if(t.isValid())
          {
            time = t;
            previousTime = t;
          }
          break;
        }
        case Chat:
        {
          LogChatElement element = getLogChatElement(message);
          if(element.isValid())
            elements.append(LogElement(element));
          break;
        }
        case Player:
        {
          LogPlayerElement element = getLogPlayerElement(message);
          if(element.isValid())
          {
            idNameMap.insert(element.getId(),element.getName());
            elements.append(LogElement(element));
          }
          break;
        }
        case PlayerRemove:
        {
          LogPlayerRemoveElement element = getLogPlayerRemoveElement(message);
          if(element.isValid())
            elements.append(LogElement(element));
          break;
        }
        case UnknownElement:
          break;
      }
      message = message.nextSiblingElement("div");
    }
    if(determinedViewerId.isEmpty())
    {
      for(int i=0;i<elements.size();i++)
      {
        if(elements[i].isPlayerElement())
        {
          determinedViewerId = elements[i].playerElement()->getId();
        }
      }
    }
    info = LogInfoElement(server,RoomInfo(room,false,0,0,time),determinedViewerId,time);
    if(info.isValid())
    {
      elements.prepend(info);
      error = NoError;
    }
    else
    {
      error = MalformedLogfileError;
    }
  }

  HtmlLogParser::HtmlLogParser()
  {
    error = Unknown;
  }

  HtmlLogParser::HtmlLogParser(const QString& filename) : HtmlLogParser()
  {
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly))
    {
      parse(file.readAll());
      file.close();
    }
    else
    {
      error = FileOpenError;
    }
  }

  HtmlLogParser::HtmlLogParser(const QByteArray& data) : HtmlLogParser()
  {
    parse(data);
  }

}
