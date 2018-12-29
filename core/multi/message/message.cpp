#include "message.h"
#include<QJsonDocument>

namespace orpg
{
  QMap<QString,BaseMessage::MessageType> Message::_stringTypeMap;

  const QMap<QString, BaseMessage::MessageType>& Message::stringTypeMap()
  {
    if(_stringTypeMap.isEmpty())
      _stringTypeMap = QMap<QString, BaseMessage::MessageType>({
                          std::pair<QString,BaseMessage::MessageType>(AssignRoleMessage::messageType,BaseMessage::TypeAssignRole),
                          std::pair<QString,BaseMessage::MessageType>(ChatMessage::messageType,BaseMessage::TypeChatMessage),
                          std::pair<QString,BaseMessage::MessageType>(CreateRoomMessage::messageType,BaseMessage::TypeCreateRoom),
                          std::pair<QString,BaseMessage::MessageType>(GiveRoleMessage::messageType,BaseMessage::TypeGiveRole),
                          std::pair<QString,BaseMessage::MessageType>(HeartbeatMessage::messageType,BaseMessage::TypeHeartbeat),
                          std::pair<QString,BaseMessage::MessageType>(JoinedRoomMessage::messageType,BaseMessage::TypeJoinedRoom),
                          std::pair<QString,BaseMessage::MessageType>(JoinRoomMessage::messageType,BaseMessage::TypeJoinRoom),
                          std::pair<QString,BaseMessage::MessageType>(KickMessage::messageType,BaseMessage::TypeKick),
                          std::pair<QString,BaseMessage::MessageType>(PlayerMessage::messageType,BaseMessage::TypePlayer),
                          std::pair<QString,BaseMessage::MessageType>(PlayerRemoveMessage::messageType,BaseMessage::TypePlayerRemove),
                          std::pair<QString,BaseMessage::MessageType>(RoomResourceMessage::messageType,BaseMessage::TypeRoomResource),
                          std::pair<QString,BaseMessage::MessageType>(RoomsMessage::messageType,BaseMessage::TypeRooms),
                          std::pair<QString,BaseMessage::MessageType>(RoomVariableMessage::messageType,BaseMessage::TypeRoomVariable),
                          std::pair<QString,BaseMessage::MessageType>(SetIdMessage::messageType,BaseMessage::TypeSetId)
                        });
    return _stringTypeMap;
  }

  Message::Message()
  {
    _wrappedMessage = nullptr;
  }

  Message::Message(const BaseMessage& message)
  {
    _wrappedMessage = message.clone();
  }

  Message::Message(const Message& other)
  {
    _wrappedMessage = other.clone();
  }

  Message::Message(const QJsonObject& object)
  {
    QString messageTypeString = object[actionField].toString();
    _wrappedMessage = nullptr;
    if(stringTypeMap().contains(messageTypeString))
    {
      MessageType type = stringTypeMap()[messageTypeString];
      switch(type)
      {
        case TypeAssignRole:
          _wrappedMessage = new AssignRoleMessage(object);
          break;
        case TypeChatMessage:
          _wrappedMessage = new ChatMessage(object);
          break;
        case TypeCreateRoom:
          _wrappedMessage = new CreateRoomMessage(object);
          break;
        case TypeGiveRole:
          _wrappedMessage = new GiveRoleMessage(object);
          break;
        case TypeHeartbeat:
          _wrappedMessage = new HeartbeatMessage(object);
          break;
        case TypeJoinedRoom:
          _wrappedMessage = new JoinedRoomMessage(object);
          break;
        case TypeJoinRoom:
          _wrappedMessage = new JoinRoomMessage(object);
          break;
        case TypeKick:
          _wrappedMessage = new KickMessage(object);
          break;
        case TypePlayer:
          _wrappedMessage = new PlayerMessage(object);
          break;
        case TypePlayerRemove:
          _wrappedMessage = new PlayerRemoveMessage(object);
          break;
        case TypeRooms:
          _wrappedMessage = new RoomsMessage(object);
          break;
        case TypeSetId:
          _wrappedMessage = new SetIdMessage(object);
          break;
        case TypeRoomVariable:
          _wrappedMessage = new RoomVariableMessage(object);
          break;
        case TypeRoomResource:
          _wrappedMessage = new RoomResourceMessage(object);
          break;
        default:
          _wrappedMessage = new UnknownMessage(object);
          break;
      }
    }
    else
    {
      _wrappedMessage = new UnknownMessage(object);
    }
  }

  Message::Message(const QByteArray& byteArray)
    : Message(QJsonDocument::fromJson(qUncompress(byteArray)).object())
  {

  }

  Message::~Message()
  {
    delete _wrappedMessage;
  }

  QJsonObject Message::toJson() const
  {
    if(_wrappedMessage)
      return _wrappedMessage->toJson();
    else
      return QJsonObject();
  }

  bool Message::isValid() const
  {
    if(_wrappedMessage)
      return _wrappedMessage->isValid();
    else
      return false;
  }

  QDateTime Message::time() const
  {
    if(_wrappedMessage)
      return _wrappedMessage->time();
    else
      return QDateTime();
  }

  BaseMessage*Message::clone() const
  {
    return _wrappedMessage ? _wrappedMessage->clone() : nullptr;
  }

  BaseMessage::MessageType Message::type() const
  {
    if(_wrappedMessage)
      return _wrappedMessage->type();
    else
      return TypeNotSet;
  }

  QString Message::room() const
  {
    return _wrappedMessage ? _wrappedMessage->room() : QString();
  }

  void Message::setRoom(const QString& room)
  {
    if(_wrappedMessage)
      _wrappedMessage->setRoom(room);
  }

  QString Message::senderId() const
  {
    return _wrappedMessage ? _wrappedMessage->senderId() : QString();
  }

  void Message::setSenderId(const QString& senderId)
  {
    if(_wrappedMessage)
      _wrappedMessage->setSenderId(senderId);
  }

  PlayerRole Message::senderRole() const
  {
    return _wrappedMessage ? _wrappedMessage->senderRole() : ROLE_NOINTRODUCTION;
  }

  void Message::setSenderRole(const PlayerRole& senderRole)
  {
    if(_wrappedMessage)
      _wrappedMessage->setSenderRole(senderRole);
  }

}
