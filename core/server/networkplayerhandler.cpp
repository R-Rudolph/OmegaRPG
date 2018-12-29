#include "networkplayerhandler.h"

namespace orpg
{
  const QMap<BaseMessage::MessageType,PlayerRole> NetworkPlayerHandler::privilegeMap({
                                                                    std::pair<BaseMessage::MessageType,PlayerRole>(BaseMessage::TypeKick,ROLE_GM)
                                                                    });

  quint32 NetworkPlayerHandler::getRoom() const
  {
    return room;
  }

  Message NetworkPlayerHandler::getPlayerMessage() const
  {
    PlayerMessage msg(name,typingStatus,QString::number(id),role);
    msg.setRoom(QString::number(room));
    msg.setSenderId(QString::number(id));
    return msg;
  }

  bool NetworkPlayerHandler::concerns(const Message &msg)
  {
    if(role==ROLE_NOINTRODUCTION)
    {
      return false;
    }
    if((QString::number(room) != msg.room()) && (!msg.room().isEmpty()))
    {
      return false;
    }
    if(msg.type()==BaseMessage::TypeChatMessage)
    {
      ChatMessage chatMessage = msg.toMessage<ChatMessage>();
      if(!chatMessage.isTo(getPlayer()))
        return false;
    }
    return true;
  }

  void NetworkPlayerHandler::disconnectedSlot()
  {
    emit disconnected(this);
  }

  bool NetworkPlayerHandler::checkRights(BaseMessage::MessageType type)
  {
    switch(type)
    {
      default:
      {
        if(!NetworkPlayerHandler::privilegeMap.contains(type))
          return true;
        return (role>=NetworkPlayerHandler::privilegeMap[type]);
      }
    }
   }

   NetworkPlayerHandler::NetworkPlayerHandler(QObject *parent) : QObject(parent)
   {

   }

   NetworkPlayerHandler::NetworkPlayerHandler(QSslSocket *socket, quint32 id, QObject *parent) : QObject(parent)
   {
     client = new NetworkHost(socket);
     this->id = id;
     this->role = ROLE_NOINTRODUCTION;
     this->room = 0;

     connect(client,&NetworkHost::disconnected,this,&NetworkPlayerHandler::disconnectedSlot);
     connect(client,&NetworkHost::receivedMessage,this,&NetworkPlayerHandler::handleMessage);
     connect(client,&NetworkHost::greetingOk,this,&NetworkPlayerHandler::requestID);
     connect(this,&NetworkPlayerHandler::sendMessageSignal,client,&NetworkHost::sendMessage);
   }

   NetworkPlayerHandler::~NetworkPlayerHandler()
   {
   }

   Player NetworkPlayerHandler::getPlayer()
   {
     Player player;
     player.id = QString::number(id);
     player.name = name;
     player.status = typingStatus;
     return player;
   }

   void NetworkPlayerHandler::disconnectSocket()
   {
     client->disconnectSocket();
   }

   void NetworkPlayerHandler::handleMessage(orpg::Message msg)
   {
     if(checkRights(msg.type()))
     {
       msg.setSenderId(QString::number(id));
       msg.setRoom(QString::number(room));
       msg.setSenderRole(role);
       emit messageReceived(this,msg);
     }
   }

   void NetworkPlayerHandler::send(const orpg::Message & message)
   {
     if(concerns(message))
     {
       emit sendMessageSignal(message);
     }
   }

   void NetworkPlayerHandler::send(const QVector<orpg::Message> &messages)
   {
     for(int i=0;i<messages.size();i++)
       send(messages[i]);
   }

   void NetworkPlayerHandler::send(const QList<Message> &messages)
   {
     foreach(const Message msg, messages)
       send(msg);
   }

}
