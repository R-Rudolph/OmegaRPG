#include "jsonmessagelogger.h"
#include <QDir>
#include <QDateTime>
#include "core/client/settings.h"

#include "core/client/log/elements/loginfoelement.h"
#include "core/client/log/elements/logchatelement.h"
#include "core/client/log/elements/logplayerelement.h"
#include "core/client/log/elements/logplayerremoveelement.h"
#include "core/client/log/elements/logstatuselement.h"

namespace orpg
{
  JsonMessageLogger::JsonMessageLogger(QObject* parent) : MessageLogger(parent)
  {

  }

  JsonMessageLogger::~JsonMessageLogger()
  {
    if(sessionRunning())
    {
      LogStatusElement element(LogStatusElement::Quit,QDateTime::currentDateTime());
      write(element);
    }
  }

  void JsonMessageLogger::disconnected()
  {
    if(sessionRunning())
    {
      LogStatusElement element(LogStatusElement::Disconnect,QDateTime::currentDateTime());
      write(element);
    }
    finishSession();
  }

  void JsonMessageLogger::writeJson(const QJsonObject& value)
  {
    file.seek(file.pos()-1);
    file.write(",");
    file.write(QJsonDocument(value).toJson(QJsonDocument::Compact));
    file.write("]");
    file.flush();
  }

  void JsonMessageLogger::write(const BaseLogElement& element)
  {
    writeJson(element.toJson());
  }

  void JsonMessageLogger::digestMessage_chat(const ChatMessage& msg)
  {
    if(sessionRunning())
    {
      if(msg.hasSenderId())
        chatMessageCount++;
      QString from = msg.senderId();
      QString to = msg.to();
      QString name = msg.name();
      QString color = msg.color();
      QString text = msg.text();
      LogChatElement chatElement(from,to,name,color,text,QDateTime::currentDateTime());
      write(chatElement);
    }
  }

  void JsonMessageLogger::digestMessage_player(const PlayerMessage& msg)
  {
    if(sessionRunning())
    {
      QString id = msg.id();
      QString newName = msg.name();
      QString oldName = idNameMap[id];
      if(!idNameMap.contains(id) || newName!=oldName)
      {
        idNameMap.insert(id,newName);
        LogPlayerElement playerElement(id,newName,QDateTime::currentDateTime());
        write(playerElement);
      }
    }
  }

  void JsonMessageLogger::digestMessage_removePlayer(const PlayerRemoveMessage& msg)
  {
    if(sessionRunning())
    {
      QString id = msg.id();
      if(idNameMap.contains(id))
      {
        LogPlayerRemoveElement playerRemoveElement(id,QDateTime::currentDateTime());
        idNameMap.remove(id);
        write(playerRemoveElement);
      }
    }
  }

  void JsonMessageLogger::digestMessage_joinedRoom(const JoinedRoomMessage& msg)
  {
    if(sessionRunning())
    {
      LogStatusElement element(LogStatusElement::LeftRoom,QDateTime::currentDateTime());
      write(element);
    }
    if(!initSession(msg))
    {
      emit error();
    }
  }

  void JsonMessageLogger::messageDigestStart(const Message&)
  {

  }

  void JsonMessageLogger::messageDigestFinish(const Message&)
  {

  }

  bool JsonMessageLogger::initSession(const JoinedRoomMessage& msg)
  {
    finishSession();
    currentUid = msg.roomInfo().uid();
    if(roomUidMap.contains(currentUid))
    {
      QString filename = roomUidMap[currentUid];
      roomUidMap.insert(currentUid,filename);
      file.setFileName(filename);
      if(file.exists())
      {
        if(file.open(QIODevice::ReadWrite))
        {
          file.seek(file.size());
          write(LogStatusElement(LogStatusElement::JoinedRoom,QDateTime::currentDateTime()));
          return true;
        }
        else
        {
          return false;
        }
      }
      else
      {
        file.setFileName(filename);
        if(file.open(QIODevice::WriteOnly))
        {
          LogInfoElement logInfo(currentServername,
                                 msg.roomInfo(),
                                 currentId,
                                 QDateTime::currentDateTime());
          file.write("[");
          file.write(QJsonDocument(logInfo.toJson()).toJson(QJsonDocument::Compact));
          file.write("]");
          write(LogStatusElement(LogStatusElement::JoinedRoom,QDateTime::currentDateTime()));
          return true;
        }
        else
        {
          return false;
        }
      }
    }
    else
    {
      QString filename = orpg::Settings::get()->getLogDir().absoluteFilePath(QString("%1.json").arg(fileDateString()));
      roomUidMap.insert(currentUid,filename);
      file.setFileName(filename);
      if(file.open(QIODevice::WriteOnly))
      {
        LogInfoElement logInfo(currentServername,
                               msg.roomInfo(),
                               currentId,
                               QDateTime::currentDateTime());
        file.write("[");
        file.write(QJsonDocument(logInfo.toJson()).toJson(QJsonDocument::Compact));
        file.write("]");
        write(LogStatusElement(LogStatusElement::JoinedRoom,QDateTime::currentDateTime()));
        return true;
      }
      else
      {
        return false;
      }
    }
  }

  void JsonMessageLogger::finishSession()
  {
    file.close();
    if(chatMessageCount==0 && file.exists())
    {
      roomUidMap.remove(currentUid);
      file.remove();
    }
    chatMessageCount = 0;
    idNameMap.clear();
  }

  QString JsonMessageLogger::fileDateString()
  {
    return QDateTime::currentDateTime().toString("yyyy-MM-dd-hhmmss");
  }

  bool JsonMessageLogger::sessionRunning()
  {
    return file.isOpen();
  }
}
