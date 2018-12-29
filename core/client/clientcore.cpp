#include "clientcore.h"
#include "core/multi/roomresources/initiativeresourcecontainer.h"
#include "core/multi/roompropertiesutility.h"

#include "core/multi/roomresources/mapresources/mapfogresourcecontainer.h"
#include "core/multi/roomresources/mapresources/maplineresourcecontainer.h"
#include "core/multi/roomresources/mapresources/mapminiatureresourcecontainer.h"
#include "core/multi/roomresources/mapresources/maptextresourcecontainer.h"

#include "core/multi/message/kickmessage.h"
#include "core/multi/message/chatmessage.h"
#include "core/multi/message/playermessage.h"
#include "core/multi/message/joinroommessage.h"
#include "core/multi/message/createroommessage.h"
#include "core/multi/message/assignrolemessage.h"
#include "core/multi/message/heartbeatmessage.h"
#include "core/multi/message/joinedroommessage.h"

#include "util/qtio.h"

namespace orpg
{

  ClientCore* ClientCore::staticCore = nullptr;
  const QSet<orpg::BaseMessage::MessageType> ClientCore::localLoopMessages({orpg::BaseMessage::TypeChatMessage,
                                                                            orpg::BaseMessage::TypeRoomResource,
                                                                            orpg::BaseMessage::TypeRoomVariable});

  PlayerRole ClientCore::getRole() const
  {
    return role;
  }

  ResourceContainerCollection *ClientCore::getResources() const
  {
    return roomResources;
  }

  RoomVariableContainerCollection *ClientCore::getVariables() const
  {
    return roomVariables;
  }

  void ClientCore::handleMessage_chat(const ChatMessage &message)
  {
    emit receivedChat(message);
  }

  void ClientCore::handleMessage_setId(const SetIdMessage &message)
  {
    id = message.id().toUInt();
    emit idChanged(id);
  }

  void ClientCore::handleMessage_player(const PlayerMessage &message)
  {
    Player newPlayer(message.id(),message.name(),message.status());
    newPlayer.role = message.role();
    //remove players of id
    bool found = false;
    for(int i=0;i<players.size();i++)
      if(players[i].id == newPlayer.id)
      {
        players.insert(i+1,newPlayer);
        players.remove(i);
        found = true;
        break;
      }
    if(!found)
    {
      players.push_back(newPlayer);
      emit statusMessage(newPlayer.name + " ("+newPlayer.id+") joined.");
    }
    emit playersChanged(players);
  }

  void ClientCore::handleMessage_playerremove(const PlayerRemoveMessage &message)
  {
    QString id = message.id();
    Player p;
    for(int i=0;i<players.size();i++)
      if(players[i].id == id)
      {
        p = players[i];
        players.remove(i);
        i--;
      }
    emit playersChanged(players);
    emit statusMessage(p.name + " ("+p.id+") left.");
  }

  void ClientCore::handleMessage_giveRole(const GiveRoleMessage &message)
  {
    setRole(message.role());
  }

  void ClientCore::handleMessage_rooms(const RoomsMessage &message)
  {
    rooms = message.rooms().toVector();
    emit roomsChanged(rooms);
  }

  void ClientCore::handleMessage_joinedRoom(const JoinedRoomMessage &message)
  {
    roomResources->clear();
    players.clear();
    emit statusMessage(QString("Entered room: '%1'").arg(message.roomInfo().name()));
    emit playersChanged(players);
  }

  void ClientCore::handleMessage_roomResource(const RoomResourceMessage &message)
  {
    roomResources->handleMessage(message);
  }

  void ClientCore::handleMessage_roomVariable(const RoomVariableMessage &message)
  {
    roomVariables->handleMessage(message);
  }

  void ClientCore::send(const Message &message)
  {
    if(!message.isValid())
      return;
    if(client->isConnected)
    {
      client->sendMessage(message);
      heartbeatTimer->start();
    }
    else if(localLoopMessages.contains(message.type()))
    {
      handleMessage(message);
    }
  }

  void ClientCore::send(const QVector<Message> &messages)
  {
    foreach(const Message& message, messages)
      send(message);
  }

  void ClientCore::send(const QList<Message> &messages)
  {
    foreach(const Message& message, messages)
      send(message);
  }

  void ClientCore::setRole(PlayerRole role)
  {
    this->role = role;
    emit roleChanged(this->role);
  }

  Player* ClientCore::getPlayerById(const QString& id)
  {
    for(int i=0;i<players.size();i++)
    {
      if(players[i].id==id)
      {
        return & players[i];
      }
    }
    return nullptr;
  }

  ClientCore *ClientCore::get()
  {
    if(ClientCore::staticCore == nullptr)
      ClientCore::staticCore = new ClientCore();
    return ClientCore::staticCore;
  }

  ClientCore::ClientCore(QObject *parent) : QObject(parent)
  {
    role = ROLE_GM;
    client = new NetworkClient;
    roomResources = new ResourceContainerCollection(this);
    roomVariables = new RoomVariableContainerCollection(this);
    heartbeatTimer = new QTimer(this);
    heartbeatTimer->setSingleShot(false);
    heartbeatTimer->setInterval(60000);
    heartbeatTimer->start();
    QObject::connect(client,&NetworkClient::receivedMessage,this,&ClientCore::handleMessage);
    QObject::connect(client,&NetworkClient::connected,this,&ClientCore::connected);
    QObject::connect(client,&NetworkClient::encrypted,this,&ClientCore::encrypted);
    QObject::connect(client,&NetworkClient::disconnected,this,&ClientCore::disconnected);
    QObject::connect(client,&NetworkClient::incorrectPeer,this,&ClientCore::incorrectServer);
    QObject::connect(client,&NetworkClient::correctPeer,this,&ClientCore::correctServer);

    QObject::connect(client,&NetworkClient::sendingError,this,&ClientCore::sendingError);
    QObject::connect(client,&NetworkClient::error,this,&ClientCore::socketError);
    QObject::connect(client,&NetworkClient::sslErrorsSignal,this,&ClientCore::sslErrors);

    QObject::connect(heartbeatTimer,&QTimer::timeout,this,&ClientCore::heartbeat);

    QObject::connect(roomResources,&ResourceContainerCollection::initiativeChanged,this,&ClientCore::initiativeChanged);
  }

  void ClientCore::kick(quint32 id)
  {
    send(KickMessage(id));
  }

  void ClientCore::correctServer()
  {
    emit statusMessage("Server is confirmed OmegaRPG Server.");
  }

  void ClientCore::incorrectServer()
  {
    emit errorMessage("Server is no OmegaRPG Server.");
  }

  void ClientCore::greetingTimeout()
  {

  }

  void ClientCore::sendClearMapMessage()
  {
    send(roomResources->getMapFogContainer()->allRemoveMessages());
    send(roomResources->getMapLineContainer()->allRemoveMessages());
    send(roomResources->getMapMiniatureContainer()->allRemoveMessages());
    send(roomResources->getMapTextContainer()->allRemoveMessages());
  }

  bool ClientCore::exportRoomProperties(const QString &filepath)
  {
    if(role>=ROLE_GM)
      return RoomPropertiesUtility::saveRoomProperties(roomVariables,roomResources,filepath);
    else
      return false;
  }

  bool ClientCore::importRoomProperties(const QString &filepath)
  {
    if(role>=ROLE_GM)
    {
      send(RoomPropertiesUtility::loadRoomProperties(filepath));
      return true;
    }
    else
      return false;
  }

  void ClientCore::activateNextInitiativeResource()
  {
    send(orpg::Message(roomResources->getInitiativeContainer()->nextMessage()));
  }

  void ClientCore::cloneInitiativeResource(int id)
  {
    Resource* resource = roomResources->getInitiativeContainer()->getResourceById(id);
    if(resource!=nullptr)
    {
      send(orpg::Message(roomResources->getInitiativeContainer()->newMessage(*resource)));
    }
  }

  void ClientCore::moveInitiativeResource(int id, int position)
  {
    send(orpg::Message(roomResources->getInitiativeContainer()->moveMessage(id, position)));
  }

  void ClientCore::sortInitiativeResources()
  {
    send(roomResources->getInitiativeContainer()->sortMessage());
  }

  void ClientCore::newInitiativeResource(const InitiativeResource &resource)
  {
    QList<InitiativeResource> list = roomResources->getInitiativeContainer()->getResources();
    InitiativeResource res = resource;
    if(list.isEmpty())
      res.setPosition(0);
    else
      res.setPosition(list.last().getPosition()+1);
    send(orpg::Message(roomResources->getInitiativeContainer()->newMessage(res)));
  }

  void ClientCore::setInitiativeResource(const InitiativeResource &resource)
  {
    send(orpg::Message(roomResources->getInitiativeContainer()->setMessage(resource)));
  }

  void ClientCore::setInitiativeResourceColor(int id, const QString &color)
  {
    InitiativeResource* resource = (InitiativeResource*) roomResources->getInitiativeContainer()->getResourceById(id);
    resource->setColor(color);
    setInitiativeResource(*resource);
  }

  void ClientCore::removeInitiativeResource(int id)
  {
    send(orpg::Message(roomResources->getInitiativeContainer()->removeMessage(id)));
  }

  void ClientCore::activateInitiativeResource(int id)
  {
    send(orpg::Message(roomResources->getInitiativeContainer()->activateMessage(id)));
  }

  void ClientCore::reconnect()
  {
    connect(hostAddr);
  }

  void ClientCore::connect(QString address)
  {
    hostAddr = address;
    rooms.clear();
    setRole(ROLE_NOINTRODUCTION);
    if(name.isEmpty())
    {
      emit errorMessage("Could not connect: Username not set");
      return;
    }
    if(status.isEmpty())
    {
      emit errorMessage("Could not connect: Typing status not set");
      return;
    }
    QString addr, port;
    QStringList list = address.split(":");
    if(list.size()==1)
    {
      addr = list[0];
      port = QString::number(Global::standardPort());
    }
    else if(list.size()==2)
    {
      addr = list[0];
      port = list[1];
    }
    else
    {
      emit errorMessage("Could not connect: Invalid address");
      return;
    }
    openrpgLogger.connect(addr);
    //sessionLogger.setServer(address);
    jsonLogger.setServer(address);
    client->connect(addr,port);
  }

  void ClientCore::disconnectFromHost()
  {
    rooms.clear();
    client->disconnect();
  }

  void ClientCore::handleMessage(orpg::Message message)
  {
    if(message.isValid())
    {
      Player* player = getPlayerById(message.senderId());
      if(player)
      {
        message.setSenderRole(player->role);
      }
      //sessionLogger.digestMessage(message);
      jsonLogger.digestMessage(message);
      openrpgLogger.digestMessage(message);
      switch(message.type())
      {
        case BaseMessage::TypeChatMessage:
          handleMessage_chat(message.toMessage<ChatMessage>());
          break;
        case BaseMessage::TypePlayer:
          handleMessage_player(message.toMessage<PlayerMessage>());
          break;
        case BaseMessage::TypePlayerRemove:
          handleMessage_playerremove(message.toMessage<PlayerRemoveMessage>());
          break;
        case BaseMessage::TypeRooms:
          handleMessage_rooms(message.toMessage<RoomsMessage>());
          break;
        case BaseMessage::TypeGiveRole:
          handleMessage_giveRole(message.toMessage<GiveRoleMessage>());
          break;
        case BaseMessage::TypeJoinedRoom:
          handleMessage_joinedRoom(message.toMessage<JoinedRoomMessage>());
          break;
        case BaseMessage::TypeRoomResource:
          handleMessage_roomResource(message.toMessage<RoomResourceMessage>());
          break;
        case BaseMessage::TypeRoomVariable:
          handleMessage_roomVariable(message.toMessage<RoomVariableMessage>());
          break;
        case BaseMessage::TypeSetId:
          handleMessage_setId(message.toMessage<SetIdMessage>());
          break;
        case BaseMessage::TypeHeartbeat:
          break;
        default:
          break;
      }
    }
    else
    {
      QtIO::err << "Received invalid message: " << QJsonDocument(message.toJson()).toJson() << endl;
    }
  }

  void ClientCore::setStatus(QString status)
  {
    this->status=status;
    player(this->status,this->name);
  }

  void ClientCore::setName(QString name)
  {
    this->name=name;
    player(this->status,this->name);
  }

  int initctr = 0;

  void ClientCore::chat(const QString &name, const QString &color, const QString &text, const QString &to)
  {
    if(client->isConnected)
    {
      send(ChatMessage(to,name,color,text));
    }
    else
    {
      handleMessage_chat(ChatMessage(to,name,color,text));
    }
  }

  void ClientCore::player(const QString &status, const QString &name)
  {
    if(this->status!=status || this->name!=name)
      send(PlayerMessage(name,status,QString(),role));
    this->status = status;
    this->name = name;
  }

  void ClientCore::connected()
  {
    openrpgLogger.connected();
    emit statusMessage("Connected to server.");
    emit connectedSignal();
  }

  void ClientCore::encrypted()
  {
    send(PlayerMessage(name,status));
    emit statusMessage("Encrypted Connection established.");
  }

  void ClientCore::disconnected()
  {
    jsonLogger.disconnected();
    openrpgLogger.disconnected();
    players.clear();
    rooms.clear();
    emit roomsChanged(rooms);
    emit playersChanged(players);
    emit statusMessage("Disconnected from server.");
    emit disconnectedSignal();
    if(role<ROLE_GM)
    {
      roomResources->clear();
      roomVariables->clear();
    }
    setRole(ROLE_GM);
  }

  void ClientCore::sendingError()
  {
    emit errorMessage("Failed to send message");
  }

  void ClientCore::socketError(QAbstractSocket::SocketError err)
  {
    switch(err)
    {
      case QAbstractSocket::ConnectionRefusedError:
        emit errorMessage("Could not connect. Server refused connection.");
        break;
      case QAbstractSocket::RemoteHostClosedError:
        emit errorMessage("Remote host closed connection.");
        break;
      case QAbstractSocket::HostNotFoundError:
        emit errorMessage("Could not resolve host address.");
        break;
      case QAbstractSocket::SocketAccessError:
      case QAbstractSocket::SocketResourceError:
        emit errorMessage("Could not access network socket.");
        break;
      case QAbstractSocket::SocketTimeoutError:
        emit errorMessage("Connection Timeout.");
        break;
      case QAbstractSocket::NetworkError:
      {
        if(!client->isConnected)
        {
          emit errorMessage("Could not connect. No Server response.");
          break;
        }
        else
          emit errorMessage("Connection lost: Socket Error " + QString::number(err));
        break;
      }
      default:
        emit errorMessage("Connection lost: Socket Error " + QString::number(err));
    }
  }

  void ClientCore::joinRoom(orpg::JoinRoomMessage room)
  {
    send(room);
  }

  void ClientCore::createRoom(QString name, QString pass, QString dmpass)
  {
    send(CreateRoomMessage(name,pass,dmpass));
  }

  void ClientCore::assignStatus(quint32 id, PlayerRole status, QString dmpass)
  {
    send(AssignRoleMessage(QString::number(id),status,dmpass));
  }

  void ClientCore::announceCurrentState()
  {
    emit roleChanged(role);
    roomVariables->announceVariables();
  }

  void ClientCore::heartbeat()
  {
    send(HeartbeatMessage());
  }

}
