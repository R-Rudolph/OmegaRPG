#include "logparser.h"

#include "elements/logchatelement.h"
#include "elements/logplayerelement.h"
#include "elements/logplayerremoveelement.h"
#include "elements/logstatuselement.h"
#include "elements/loginfoelement.h"

#include<QJsonArray>

namespace orpg
{
  const QString LogParser::allString = "all";
  const QString LogParser::gmString = "dm";
  const QString LogParser::mixedString = "mixed";
  const QString LogParser::mainChannelString = "Main Channel";
  const QString LogParser::gmChannelString = "GM";
  const QString LogParser::mixedChannelString = "Mixed";

  QString LogParser::infoTextHtml(const QDateTime& time, const QString& text)
  {
    return QString("<div>[%1] <span style=\"color:%2;\">%3</span></div>").arg(time.toString("hh:mm"),
                                                                              infoColor,
                                                                              text);
  }

  void LogParser::appendToMap(const QString& key, const QString& value, QMap<QString, QString>& map)
  {
    if(map.contains(key))
      map[key] += value;
    else
      map.insert(key,value);
  }

  QString LogParser::getName(const QString& id)
  {
    return idNameMap.value(id);
  }

  QString LogParser::chatTextHtml(const LogChatElement& element, bool explicitTo)
  {
    /*
    QStringList rawMessage({"<div>[%1] <strong>",
                            element.getFrom().isEmpty() ? "" : "(%2) ",
                            "%3: </strong>",
                            explicitTo ? "<i>(to %4)</i> " : "",
                            "<span style=\"color:%5;\">%6</span></div>"});
    */
    QString raw("<div>[%1] <strong>%2%3: </strong>"
                "%4<span style=\"color:%5;\">%6</span></div>");
    return raw.arg(element.getTime().toString("hh:mm"), //time
                   element.getFrom().isEmpty() ? "" : QString("(%1) ").arg(element.getFrom()), //id
                   element.getName(), //name
                   explicitTo ? QString("<i>(to %1)</i>").arg(getName(element.getTo())) : "", //'to' part
                   element.getColor(), //color
                   element.getText() //text
                   );

    /*
    QString html = rawMessage.join("");
    html = html.arg(element.getTime().toString("hh:mm"));
    if(!element.getFrom().isEmpty())
    {
      html = html.arg(element.getFrom());
    }
    html = html.arg(element.getName());
    if(explicitTo)
    {
      html = html.arg(getName(element.getTo()));
    }
    html = html.arg(element.getColor(),element.getText());
    return html;
    */
  }

  QMap<QString, QString> LogParser::getChannels()
  {
    QMap<QString,QString> result;
    QMap<QString,QString> currentIdNameMap;
    for(int i=0;i<elements.size();i++)
    {
      {
        const LogElement& element = elements[i];
        if(!element.isValid())
          continue;
        switch(element.getElementType())
        {
          case LogElement::Info:
          {
            const LogInfoElement* subelement = element.infoElement();
            QString html = QString("<div><strong>Server: </strong>%1</div>"
                                   "<div><strong>Room: </strong>%2</div>"
                                   "<div><strong>Date: </strong>%3</div>"
                                   "<br/>").arg(subelement->getServerName(),
                                                subelement->getRoomInfo().name(),
                                                subelement->getTime().toString("yyyy-MM-dd hh:mm:ss"));
            appendToMap(allString,html,result);
            appendToMap(mixedString,html,result);
            break;
          }
          case LogElement::Chat:
          {
            const LogChatElement* subelement = element.chatElement();
            QString channel = subelement->getTo();
            if(channel==info.getViewerID())
              channel = subelement->getFrom();
            if(channel.isEmpty())

              channel = allString;
            appendToMap(channel,
                        chatTextHtml(*subelement),
                        result);
            if(channel==allString)
            {
              appendToMap(mixedString,
                          chatTextHtml(*subelement),
                          result);
            }
            else
            {
              appendToMap(mixedString,
                          chatTextHtml(*subelement,true),
                          result);
            }
            break;
          }
          case LogElement::Player:
          {
            const LogPlayerElement* subelement = element.playerElement();
            QString html;
            if(currentIdNameMap.contains(subelement->getId()))
            {
              QString previousName = currentIdNameMap[subelement->getId()];
              QString text = QString("(%1) %2 is now known as (%3) %4.").arg(subelement->getId(),previousName,subelement->getId(),subelement->getName());
              html = infoTextHtml(subelement->getTime(),text);
              currentIdNameMap.insert(subelement->getId(),subelement->getName());
            }
            else
            {
              QString text = QString("(%1) %2 entered the room.").arg(subelement->getId(),subelement->getName());
              html = infoTextHtml(subelement->getTime(),text);
              currentIdNameMap.insert(subelement->getId(),subelement->getName());
            }
            appendToMap(allString,html,result);
            appendToMap(mixedString,html,result);
            break;
          }
          case LogElement::PlayerRemove:
          {
            const LogPlayerRemoveElement* subelement = element.playerRemoveElement();
            QString name = currentIdNameMap[subelement->getId()];
            QString text = QString("(%1) %2 left the room.").arg(subelement->getId(),name);
            QString html = infoTextHtml(subelement->getTime(),text);
            appendToMap(allString, html, result);
            appendToMap(mixedString, html, result);
            currentIdNameMap.remove(subelement->getId());
            break;
          }
          case LogElement::Status:
          {
            const LogStatusElement* subelement = element.statusElement();
            if(subelement->getStatus()==LogStatusElement::JoinedRoom)
            {
              currentIdNameMap.clear();
            }
            QString html = infoTextHtml(subelement->getTime(),subelement->getStatusString());
            appendToMap(allString, html, result);
            appendToMap(mixedString, html, result);
            break;
          }
          default:
            break;
        }
      }
    }
    return result;
  }

  LogParser::LogParser()
  {
    infoColor = "#ff8000";
    error = LogParser::Unknown;
    numberChatMessages = 0;
    idNameMap.insert(gmString,gmChannelString);
    idNameMap.insert(allString,mainChannelString);
    idNameMap.insert("",mainChannelString);
    idNameMap.insert(mixedString,mixedChannelString);
  }

  LogParser::~LogParser()
  {

  }

  QList<QPair<QString, QString> > LogParser::getHtmlLogByChannels()
  {
    QMap<QString,QString> channels = getChannels();
    QList< QPair<QString, QString> > result;
    QString allChannel = channels[allString];
    QString dmChannel = channels[gmString];
    QString mixedChannel = channels[mixedString];
    channels.remove(allString);
    channels.remove(gmString);
    channels.remove(mixedString);
    if(allChannel.size()>0)
      result.append(QPair<QString, QString>(mainChannelString,allChannel));
    if(dmChannel.size()>0)
      result.append(QPair<QString, QString>(gmChannelString,dmChannel));
    for(auto i = channels.begin();i!=channels.end();++i)
    {
      QString name = QString("(%1) %2").arg(i.key(),idNameMap.value(i.key()));
      result.append(QPair<QString, QString>(name,i.value()));
    }
    if(mixedChannel.size()>0)
      result.append(QPair<QString, QString>(mixedChannelString,mixedChannel));
    return result;
  }

  QString LogParser::toHtmlWithChannels_Experimental()
  {
    typedef QPair<QString, QString> chan;
    QList< chan > channels = getHtmlLogByChannels();
    QString head =
        "<head>"
        "<meta charset=\"utf-8\"/>"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
        "<style>"
        "body {font-family: Arial;}"

        "/* Style the tab */"
        ".tab {"
            "overflow: hidden;"
            "border: 1px solid #ccc;"
            "background-color: #f1f1f1;"
        "}"

        /* Style the buttons inside the tab */
        ".tab button {"
            "background-color: inherit;"
            "float: left;"
            "border: none;"
            "outline: none;"
            "cursor: pointer;"
            "padding: 14px 16px;"
            "transition: 0.3s;"
            "font-size: 17px;"
        "}"

        /* Change background color of buttons on hover */
        ".tab button:hover {"
            "background-color: #ddd;"
        "}"

        "/* Create an active/current tablink class */"
        ".tab button.active {"
            "background-color: #ccc;"
        "}"

        /* Style the tab content */
        ".tabcontent {"
            "display: none;"
            "padding: 6px 12px;"
            "border: 1px solid #ccc;"
            "border-top: none;"
        "}"
        "</style>"
        "</head>";
    QString script =
        "<script>"
        "function openTab(evt, cityName) {"
            "var i, tabcontent, tablinks;"
            "tabcontent = document.getElementsByClassName(\"tabcontent\");"
            "for (i = 0; i < tabcontent.length; i++) {"
                "tabcontent[i].style.display = \"none\";"
            "}"
            "tablinks = document.getElementsByClassName(\"tablinks\");"
            "for (i = 0; i < tablinks.length; i++) {"
                "tablinks[i].className = tablinks[i].className.replace(\" active\", \"\");"
            "}"
            "document.getElementById(cityName).style.display = \"block\";"
            "evt.currentTarget.className += \" active\";"
        "}"
        "</script>";

    QString tabs = "<div class=\"tab\">%1</div>";
    QString tabLine = "<button class=\"tablinks\" onclick=\"openTab(event, '%1')\">%1</button>";
    QString contentLine = "<div id=\"%1\" class=\"tabcontent\">%2</div>";

    QString tabLines;
    QString contentLines;
    foreach( chan channel, channels)
    {
      tabLines.append(tabLine.arg(channel.first));
      contentLines.append(contentLine.arg(channel.first,channel.second));
    }
    return "<!DOCTYPE html><html>"+head+"<body>"+script+tabs.arg(tabLines)+contentLines+"</body></html>";
  }

  QJsonArray LogParser::toJson() const
  {
    QJsonArray array;
    for(int i=0;i<elements.size();i++)
    {
      array.append(elements[i].toJson());
    }
    return array;
  }

  bool LogParser::isValid()
  {
    if(error == LogParser::NoError)
      return true;
    else
      return false;
  }

  QString LogParser::getRoomName() const
  {
    return info.getRoomInfo().name();
  }

  QString LogParser::getServerName() const
  {
    return info.getServerName();
  }

  QDateTime LogParser::getTime() const
  {
    return info.getTime();
  }

  const QVector<LogElement>&LogParser::getElements()
  {
    return elements;
  }

  QString LogParser::getUID() const
  {
    return info.getTime().toString(Qt::ISODate)+":"+info.getRoomInfo().name();
  }
}

