#include "messagelogger.h"
#include<QSet>

namespace orpg {

  const QSet<BaseMessage::MessageType> MessageLogger::handledMessageTypes({
                                                                            BaseMessage::TypeChatMessage,
                                                                            BaseMessage::TypePlayer,
                                                                            BaseMessage::TypePlayerRemove,
                                                                            BaseMessage::TypeJoinedRoom,
                                                                            BaseMessage::TypeSetId,
                                                                          });

  void MessageLogger::setServer(const QString& value)
  {
    currentServername = value;
  }

  void MessageLogger::digestMessage_setID(const SetIdMessage& message)
  {
    finishSession();
    chatMessageCount = 0;
    currentId = message.id();
  }

  void MessageLogger::messageDigestStart(const Message&)
  {

  }

  void MessageLogger::messageDigestFinish(const Message&)
  {

  }

  MessageLogger::MessageLogger(QObject* parent) : QObject(parent)
  {
    chatMessageCount = 0;
  }

  void MessageLogger::digestMessage(const Message& msg)
  {
    BaseMessage::MessageType type = msg.type();
    if(handledMessageTypes.contains(type))
    {
      messageDigestStart(msg);
      switch(type)
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
          digestMessage_setID(msg.toMessage<SetIdMessage>());
          break;
        default:
          break;
      }
      messageDigestFinish(msg);
    }
  }
}
