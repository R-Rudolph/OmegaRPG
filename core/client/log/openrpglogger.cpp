#include "openrpglogger.h"

namespace orpg
{
  QString OpenRpgLogger::escapeString(QString text)
  {
    text.replace("ä","&auml;");
    text.replace("Ä","&Auml;");
    text.replace("Ö","&Ouml;");
    text.replace("ö","&ouml;");
    text.replace("Ü","&Uuml;");
    text.replace("ü","&uuml;");
    text.replace("ß","&szlig;");
    return text;
  }

  void OpenRpgLogger::writeLineBeginning()
  {
    ts << "<div class=\"post\">";
    ts << "[" << QDateTime::currentDateTime().toString("ddd MMM d hh:mm:ss yyyy") << "] : ";
  }

  void OpenRpgLogger::writeLineEnding()
  {
    ts << "</div>\n";
  }

  void OpenRpgLogger::digestMessage_chat(const ChatMessage &message)
  {
    if(!message.hasSenderId())
    {
      writeLineBeginning();
      ts << "<font color=\"#ff8000\">" << escapeString(message.text()) << "</font>";
      writeLineEnding();
      return;
    }
    writeLineBeginning();
    ts << "<b>(" << message.senderId() << ") " << escapeString(message.name()) << "</b>: ";
    QString to = message.to();
    if(!to.isEmpty() && to!="all")
    {
      ts << " (whispering to ";
      if(to=="dm")
        ts << "GM";
      else
        ts << idPlayerMap[to];
      ts << ") ";
      writeLineEnding();
    }
    ts << "<font color=\""+message.color()+"\">" << escapeString(message.text()) << "</font>";
  }

  void OpenRpgLogger::digestMessage_player(const PlayerMessage &message)
  {
    QString id = message.id();
    QString name = message.name();
    if(idPlayerMap.contains(id))
    {
      idPlayerMap.insert(message.id(),message.name());
    }
    else
    {
      if(!id.isEmpty())
      {
        writeLineBeginning();
        ts << "<font color=\"#ff8000\">("<<id<<") "<<name<<" (enter): "<<QDateTime::currentDateTime().toString("hh:mm")<<"</font>";
        writeLineEnding();
        idPlayerMap.insert(id,name);
      }
    }
  }

  void OpenRpgLogger::digestMessage_joinedRoom(const JoinedRoomMessage &message)
  {
    idPlayerMap.clear();
    QString roomName = message.roomInfo().name();
    writeLineBeginning();
    ts << "<font color=\"#ff8000\">Moving to room '" + roomName + "'..</font>";
    writeLineEnding();
  }

  void OpenRpgLogger::digestMessage_removePlayer(const PlayerRemoveMessage &message)
  {
    QString id = message.id();
    if(idPlayerMap.contains(id))
    {
      if(!id.isEmpty())
      {
        QString name = idPlayerMap[id];
        writeLineBeginning();
        ts << "<font color=\"#ff8000\">("<<id<<") "<<name<<" (exit): "<<QDateTime::currentDateTime().toString("hh:mm")<<"</font>";
        writeLineEnding();
        idPlayerMap.remove(id);
      }
    }
  }

  bool OpenRpgLogger::openFile()
  {
    if(!orpg::Settings::get()->openRpgLogEnabled)
      return false;
    QDir dir(orpg::Settings::get()->getOpenRpgLogLocation());
    if(!dir.exists())
    {
      dir.mkpath(orpg::Settings::get()->getOpenRpgLogLocation());
    }
    file.setFileName(dir.filePath("Log -"+QDateTime::currentDateTime().toString("yyyy-MM-dd")+".html"));
    return file.open(QIODevice::Append);
  }

  OpenRpgLogger::OpenRpgLogger()
  {
    ts.setDevice(&file);
  }

  void OpenRpgLogger::connect(const QString addr)
  {
    if(openFile())
    {
      writeLineBeginning();
      ts << "<font color=\"#ff8000\">Locating server at " << addr << "...</font>";
      writeLineEnding();
      file.close();
    }
  }

  void OpenRpgLogger::connected()
  {
    if(openFile())
    {
      writeLineBeginning();
      ts << "<font color=\"#ff8000\">Game connected!</font>";
      writeLineEnding();
      file.close();
    }
  }

  void OpenRpgLogger::disconnected()
  {
    if(openFile())
    {
      writeLineBeginning();
      ts << "<font color=\"#ff0000\">Game disconnected!</font>";
      writeLineEnding();
      file.close();
    }
  }

  void OpenRpgLogger::digestMessage(const Message &msg)
  {
    bool opened = openFile();
    switch(msg.type())
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
      default:
        break;
    }
    if(opened)
    {
      file.close();
    }
  }

}
