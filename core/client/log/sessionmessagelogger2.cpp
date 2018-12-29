#include "sessionmessagelogger2.h"

namespace orpg
{
  QString SessionMessageLogger2::getChannel(const QString from, const QString &to)
  {
    if(to.isEmpty())
      return "all";
    else if(to==currentId)
      return from;
    else
      return to;
  }

  bool SessionMessageLogger2::init()
  {
    if(!orpg::Settings::get()->logEnabled)
      return false;
    if(!file.isOpen())
    {
      file.setFileName(currentFilename);
      if(file.open(QIODevice::WriteOnly))
      {
        xml.setAutoFormatting(true);
        xml.setAutoFormattingIndent(2);
        xml.setDevice(&file);
        xml.setCodec("UTF-8");
        //set up html
        xml.writeStartElement("html");
        QString wrapSpan(const QString& text, const QString& spanClass="", const QString& style="");
        xml.writeStartElement("head");
        xml.writeStartElement("meta");
        xml.writeAttribute("charset","utf-8");
        xml.writeEndElement();
        xml.writeEndElement();
        xml.writeStartElement("body");
        //write server info
        xml.writeStartElement("div");
        xml.writeAttribute("class","server");
        xml.writeAttribute("data-server",serverName);
        xml.writeStartElement("strong");
        xml.writeCharacters("Server: ");
        xml.writeEndElement();
        if(serverName.isEmpty())
          xml.writeCharacters("<Not Given>");
        else
          xml.writeCharacters(serverName);
        xml.writeEndElement();
        //write room info
        xml.writeStartElement("div");
        xml.writeAttribute("class","room");
        xml.writeAttribute("data-room",roomName);
        xml.writeStartElement("strong");
        xml.writeCharacters("Room: ");
        xml.writeEndElement();
        if(roomName.isEmpty())
          xml.writeCharacters("<Not Given>");
        else
          xml.writeCharacters(roomName);
        xml.writeEndElement();
        //write time info
        xml.writeStartElement("div");
        xml.writeAttribute("class","time");
        xml.writeAttribute("data-time",dtBegin.toString("yyyy-MM-dd hh:mm:ss"));
        xml.writeStartElement("strong");
        xml.writeCharacters("Date: ");
        xml.writeEndElement();
        xml.writeCharacters(dtBegin.toString("yyyy-MM-dd hh:mm:ss"));
        xml.writeEndElement();
        //line break
        xml.writeEmptyElement("br");
      }
      else
        return false;
    }
    return true;
  }

  void SessionMessageLogger2::writePlayer(const QString &text, const QString &name, const QString &id)
  {
    if(init())
    {
      xml.writeStartElement("div");
      xml.writeAttribute("class","player");
      xml.writeAttribute("data-id",id);
      xml.writeAttribute("data-name",name);
      xml.writeCharacters("[");
      xml.writeStartElement("span");
      xml.writeAttribute("class","time");
      xml.writeCharacters(QDateTime::currentDateTime().toString("hh:mm"));
      xml.writeEndElement();
      xml.writeCharacters("] ");
      xml.writeStartElement("span");
      xml.writeAttribute("class","text");
      xml.writeAttribute("style","color:#ff8000;");
      xml.writeCharacters(text);
      xml.writeEndElement();
      xml.writeEndElement();
    }
  }

  void SessionMessageLogger2::writeHtml(QString text)
  {
    text.replace("\n","<br/>");
    QXmlStreamReader reader("<xml>"+text+"</xml>");
    while(!reader.atEnd())
    {
      parse(reader,false);
    }
  }

  void SessionMessageLogger2::parse(QXmlStreamReader &reader, bool openedTag)
  {
    bool hasContent = false;
    while(!reader.atEnd())
    {
      switch(reader.readNext())
      {
        case QXmlStreamReader::StartElement:
        {
          if(reader.name()=="span")
          {
            hasContent = true;
            xml.writeCurrentToken(reader);
            parse(reader,true);
          }
          else if(reader.name()=="br")
          {
            hasContent = true;
            xml.writeStartElement("br");
            xml.writeEndElement();
          }
          else
          {
            hasContent = true;
            parse(reader,false);
          }
          break;
        }
        case QXmlStreamReader::EndElement:
          if(reader.name()=="span")
          {
            if(openedTag)
            {
              if(!hasContent)
                xml.writeCharacters("");
              xml.writeEndElement();
            }
            return;
          }
          else
            break;
        case QXmlStreamReader::Characters:
          hasContent = true;
          xml.writeCurrentToken(reader);
          break;
        default:
          break;
      }
    }
    if(openedTag)
    {
      if(!hasContent)
        xml.writeCharacters("");
      xml.writeEndElement();
    }
  }

  void SessionMessageLogger2::digestMessage_chat(const ChatMessage &msg)
  {

    if(init() && firstRoomEntered)
    {
      //open line
      xml.writeStartElement("div");
      xml.writeAttribute("class","chat");
      xml.writeAttribute("data-channel",getChannel(msg.senderId(),msg.to()));
      xml.writeAttribute("data-to",msg.to());
      xml.writeAttribute("data-from",msg.senderId());
      //write time
      xml.writeCharacters("[");
      xml.writeStartElement("span");
      xml.writeAttribute("class","time");
      xml.writeCharacters(QDateTime::currentDateTime().toString("hh:mm"));
      xml.writeEndElement();
      xml.writeCharacters("] ");
      //write id/name
      xml.writeStartElement("strong");
      if(msg.hasSenderId())
      {
        chatMessageReceived = true;
        xml.writeCharacters("(");
        xml.writeCharacters(msg.senderId());
        xml.writeCharacters(") ");
      }
      xml.writeStartElement("span");
      xml.writeAttribute("class","name");
      xml.writeCharacters(msg.name());
      xml.writeEndElement();
      xml.writeCharacters(": ");
      xml.writeEndElement();
      //write to whom
      if(playerMap.contains(msg.to()))
      {
        xml.writeStartElement("em");
        xml.writeCharacters("(to ");
        xml.writeCharacters(playerMap[msg.to()]);
        xml.writeCharacters(") ");
        xml.writeEndElement();
      }
      //write text
      xml.writeStartElement("span");
      xml.writeAttribute("class","text");
      xml.writeAttribute("data-color",msg.color());
      xml.writeAttribute("style","color:"+msg.color()+";");
      writeHtml(msg.text());
      xml.writeEndElement();
      //close line
      xml.writeEndElement();
    }
    //writeChat(msg.json["id"].toString(),msg.json["name"].toString(),msg.json["color"].toString(),msg.json["text"].toString(),msg.json["to"].toString(),msg.json["cid"].toInt());
  }

  void SessionMessageLogger2::digestMessage_player(const PlayerMessage& msg)
  {
    if(firstRoomEntered)
    {
      QString id = msg.id();
      QString newName = msg.name();
      if(playerMap.contains(id))
      {
        if(playerMap[id]!=newName)
        {
          writePlayer(playerMap[id]+" ("+id+") is now known as "+newName+".",newName,id);
          playerMap.insert(id,newName);
        }
      }
      else
      {
        writePlayer(newName+" ("+id+") joined.",newName,id);
        playerMap.insert(id,newName);
      }
    }
  }

  void SessionMessageLogger2::digestMessage_removePlayer(const PlayerRemoveMessage& msg)
  {
    QString id = msg.id();
    if(init() && playerMap.contains(id))
    {
      xml.writeStartElement("div");
      xml.writeAttribute("class","player_remove");
      xml.writeAttribute("data-id",id);
      xml.writeCharacters("[");
      xml.writeStartElement("span");
      xml.writeAttribute("class","time");
      xml.writeCharacters(QDateTime::currentDateTime().toString("hh:mm"));
      xml.writeEndElement();
      xml.writeCharacters("] ");
      xml.writeStartElement("span");
      xml.writeAttribute("class","text");
      xml.writeAttribute("style","color:#ff8000;");
      xml.writeCharacters(playerMap[id]+" ("+id+") left.");
      xml.writeEndElement();
      xml.writeEndElement();
      playerMap.remove(id);
    }
  }

  void SessionMessageLogger2::digestMessage_joinedRoom(const JoinedRoomMessage& msg)
  {
    cleanup();
    playerMap.clear();
    playerMap.insert("dm","DM");
    firstRoomEntered = true;
    chatMessageReceived = false;
    currentFilename = orpg::Settings::get()->getLogLocation()+QDateTime::currentDateTime().toString("yyyy-MM-dd-hhmmss")+".html";
    roomName = msg.roomInfo().name();
    dtBegin = QDateTime::currentDateTime();
    init();
  }

  void SessionMessageLogger2::digestMessage_setId(const SetIdMessage& msg)
  {
    cleanup();
    chatMessageReceived = false;
    currentId = msg.id();
    currentFilename = QString();
  }

  void SessionMessageLogger2::cleanup()
  {
    if(file.isOpen())
    {
      xml.writeEndElement();
      xml.writeEndElement();
      file.close();
      if(!chatMessageReceived)
        file.remove();
    }
    firstRoomEntered = false;
    chatMessageReceived = false;
  }

  SessionMessageLogger2::SessionMessageLogger2()
  {
    firstRoomEntered = false;
    chatMessageReceived = false;
  }

  SessionMessageLogger2::~SessionMessageLogger2()
  {
    cleanup();
  }

  void SessionMessageLogger2::setServer(const QString &serverName)
  {
    cleanup();
    this->serverName = serverName;
  }

  void SessionMessageLogger2::digestMessage(const Message &msg)
  {
    BaseMessage::MessageType messageType = msg.type();
    switch(messageType)
    {
      case BaseMessage::TypeChatMessage:
        digestMessage_chat(msg.toMessage<ChatMessage>());
        break;
      case BaseMessage::TypePlayer:
        digestMessage_player(msg.toMessage<PlayerMessage>());
        break;
      case BaseMessage::TypePlayerRemove:
        digestMessage_removePlayer(msg.toMessage<PlayerRemoveMessage>());
        break;
      case BaseMessage::TypeJoinedRoom:
        digestMessage_joinedRoom(msg.toMessage<JoinedRoomMessage>());
        break;
      case BaseMessage::TypeSetId:
        digestMessage_setId(msg.toMessage<SetIdMessage>());
        break;
      default:
        break;
    }
    if(file.isOpen())
      file.flush();
  }

}
