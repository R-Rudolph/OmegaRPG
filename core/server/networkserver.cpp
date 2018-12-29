#include "networkserver.h"
#include "util/qtio.h"
#include "core/multi/util/global.h"

namespace orpg
{
  bool NetworkServer::typesRegistered = false;

  void NetworkServer::registerTypes()
  {
    if(typesRegistered)
      return;
    typesRegistered = true;
    qRegisterMetaType< QMap<int,orpg::Room*> >("QMap<int,orpg::Room*>");
    qRegisterMetaType< orpg::Message >("orpg::Message");
  }

  ServerSettings *NetworkServer::getSettings() const
  {
    return settings;
  }

  void NetworkServer::handleMessage_player(PlayerMessage msg, NetworkPlayerHandler &handler)
  {
    QString name = msg.name();
    QString status = msg.status();
    if(handler.role == ROLE_NOINTRODUCTION)
    {
      handler.role = ROLE_LURKER;
      handler.name = msg.name();
      handler.room = 0;
      handler.typingStatus = status;
      handler.send(SetIdMessage(handler.id));
      enterRoom(handler, 0, true);
      if(!settings->serverMessage.isEmpty())
      {
        handler.send(serverChat(settings->serverMessage,QString::number(handler.id),"0"));
      }
      sendAll(handler.getPlayerMessage());
    }
    else
    {
      handler.name = msg.name();
      handler.typingStatus = status;
      sendAll(handler.getPlayerMessage());
    }
  }

  void NetworkServer::handleMessage_chat(ChatMessage msg, NetworkPlayerHandler&)
  {
    sendAll(msg);
  }

  void NetworkServer::handleMessage_assignStatus(AssignRoleMessage msg, NetworkPlayerHandler &)
  {
    QString dmpass = msg.dmpass();
    QString id = msg.id();
    PlayerRole role = msg.role();
    bool ok;
    NetworkPlayerHandler* targetPlayer = getPlayer(id.toInt(&ok));
    if(!ok || (targetPlayer==NULL))
      return;
    quint32 room = targetPlayer->getRoom();
    if(room==msg.room().toUInt(&ok))
    {
      if(!ok)
        return;
      if(role <= ROLE_GM)
      { //behaviour for becoming GM or less
        if((msg.senderRole() >= ROLE_GM
                || ( !rooms[room]->_dmpassword.isEmpty() && (dmpass == rooms[room]->_dmpassword)))
             && msg.role()<=ROLE_GM)
        {
          assignStatus(*targetPlayer,role);
        }
      }
      else if (role == ROLE_ADMIN)
      { //behaviour for becoming admin
        if(!settings->adminPassword.isEmpty() &&
           (dmpass == settings->adminPassword))
        {
          assignStatus(*targetPlayer,role);
        }
      }
    }
  }

  void NetworkServer::handleMessage_createRoom(CreateRoomMessage msg, NetworkPlayerHandler &handler)
  {
    QString name = msg.name();
    QString password = msg.password();
    QString dmpassword = msg.dmPassword();
    if(name.isEmpty())
      return;
    createRoom(handler,name,password,dmpassword);
  }

  void NetworkServer::handleMessage_joinRoom(JoinRoomMessage msg, NetworkPlayerHandler &handler)
  {
    int room = msg.roomId();
    QString password = msg.password();
    if(!rooms.contains(room))
    {
      handler.send(serverChat("Room does not exist anymore.",QString::number(handler.id),QString::number(handler.room)));
      return;
    }
    if(rooms[msg.roomId()]->_password==password)
    {
      enterRoom(handler,msg.roomId(), false);
    }
    else
    {
      handler.send(serverChat("Incorrect password.",QString::number(handler.id),QString::number(handler.room)));
    }
  }

  void NetworkServer::handleMessage_kick(KickMessage msg, NetworkPlayerHandler &handler)
  {
    bool ok;
    quint32 id = msg.id().toUInt(&ok);
    if(!ok)
      return;
    NetworkPlayerHandler* kickedPlayer = getPlayer(id);
    if(kickedPlayer==NULL)
      return;
    if(kickedPlayer->room!=handler.room)
      return;
    kick(kickedPlayer,handler);
  }

  void NetworkServer::handleMessage_roomResource(RoomResourceMessage msg, NetworkPlayerHandler&)
  {
    bool ok;
    int room = msg.room().toInt(&ok);
    if(ok && rooms.contains(room))
    {
      rooms[room]->resources->handleMessage(msg);
    }
  }

  void NetworkServer::handleMessage_roomVariable(RoomVariableMessage msg, NetworkPlayerHandler&)
  {
    bool ok;
    int room = msg.room().toInt(&ok);
    if(ok && rooms.contains(room))
    {
      rooms[room]->variables->handleMessage(msg);
    }
  }

  void NetworkServer::handleMessage_heartbeat(HeartbeatMessage message, NetworkPlayerHandler& handler)
  {
    handler.send(message);
  }

  void NetworkServer::enterRoom(NetworkPlayerHandler &handler, quint32 room, bool introduction)
  {
    if(!introduction)
    {
      if(handler.room == room)
        return;
      quint32 prevRoom = handler.room;
      handler.room = room;
      removePlayer(&handler,prevRoom);
    }
    handler.room = room;
    handler.role = ROLE_LURKER;
    handler.send(GiveRoleMessage(ROLE_LURKER));
    handler.send(JoinedRoomMessage(rooms[room]->info()));
    for(QSet<NetworkPlayerHandler*>::Iterator i=players.begin();i!=players.end();i++)
    {
      NetworkPlayerHandler* p = *i;
      if(p->room==room)
      {
        handler.send(p->getPlayerMessage());
        p->send(handler.getPlayerMessage());
      }
    }
    handler.send(rooms[room]->resources->getAllResourceMessages());
    handler.send(rooms[room]->variables->getAllVariableMessages());
    broadcastRooms();
  }

  void NetworkServer::assignStatus(NetworkPlayerHandler &handler, const PlayerRole &role)
  {
    handler.role = role;
    Message playerMessage = PlayerMessage(handler.name,handler.typingStatus,QString::number(handler.id),role);
    playerMessage.setRoom(QString::number(handler.room));
    sendAll(playerMessage);
    handler.send(GiveRoleMessage(role));
  }

  void NetworkServer::createRoom(NetworkPlayerHandler &handler, QString name, QString password, QString dmpassword)
  {
    int roomID = roomCtr;
    addRoom(name,password,dmpassword);
    sendAll(serverChat(handler.name + " created room '"+name+"'","all","0"));
    enterRoom(handler,roomID, false);
    assignStatus(handler,ROLE_GM);
    broadcastRooms();
  }

  void NetworkServer::broadcastRooms()
  {
    foreach (Room* room, rooms)
    {
      room->_numPlayers = 0;
    }
    foreach (NetworkPlayerHandler* player, players)
    {
      rooms[player->room]->_numPlayers = rooms[player->room]->_numPlayers+1;
    }
    QList<RoomInfo> roomInfo;
    foreach (Room* room, rooms)
    {
      if(room->_id!=0 && room->_numPlayers==0)
      {
        rooms.remove(room->_id);
        delete room;
      }
      else
        roomInfo.append(room->info());
    }
    sendAll(RoomsMessage(roomInfo));
    emit roomsChanged(rooms);
  }

  void NetworkServer::removePlayer(NetworkPlayerHandler *player, quint32 room)
  {
    if(player->role != ROLE_NOINTRODUCTION)
    {
      Message msg = PlayerRemoveMessage(QString::number(player->id));
      msg.setRoom(QString::number(room));
      sendAll(msg);
    }
  }

  void NetworkServer::kick(NetworkPlayerHandler *kickedPlayer, NetworkPlayerHandler &kicker)
  {
    sendAll(serverChat(kicker.name + " kicked "+kickedPlayer->name+".","all",QString::number(kicker.room)));
    enterRoom(*kickedPlayer,0,false);
  }

  void NetworkServer::init()
  {
    idctr = 1;
    roomCtr = 0;
    foreach (NetworkPlayerHandler* player, players)
    {
      player->disconnectSocket();
      delete player;
    }
    players.clear();
    foreach(Room* room, rooms.values())
      delete room;
    rooms.clear();
    addRoom("Lobby","",settings->getAdminPassword(),Global::lobbyFilepath());
  }

  orpg::Message NetworkServer::serverChat(const QString &text, const QString &to, const QString &room)
  {
    Message msg = ChatMessage(to,"Server","#000000",text);
    msg.setRoom(room);
    return msg;
  }

  NetworkPlayerHandler *NetworkServer::getPlayer(quint32 id)
  {
    foreach(NetworkPlayerHandler* nph, players)
    {
      if(nph->id == id)
        return nph;
    }
    return NULL;
  }

  NetworkServer::NetworkServer(QObject *parent, const ServerSettings& settings) : QObject(parent)
  {
    registerTypes();
    this->settings = new ServerSettings(settings,this);
    metaServerClient = new MetaServerClient(this);
    metaServerClient->setHttpMetaServerUrls(this->settings->getMetaServers());
    server = new SslServer(this);

    connect(server,&SslServer::sslConnectionReady,this,&NetworkServer::acceptConnection);

    connect(this->settings,&ServerSettings::certificateChainChanged,server,&SslServer::setCertificateChain);
    connect(this->settings,&ServerSettings::privateKeyChanged,server,&SslServer::setPrivateKey);

    metaServerTimer = new QTimer(this);
    connect(metaServerTimer,&QTimer::timeout,this,&NetworkServer::metaServerTimerTimeout);
  }

   NetworkServer::~NetworkServer()
   {
     foreach (NetworkPlayerHandler* player, players)
       delete player;
     players.clear();
   }

   QVector<Player> NetworkServer::getPlayers(quint32 room)
   {
     QVector<Player> p;
     QSetIterator<NetworkPlayerHandler*> i(players);
     while(i.hasNext())
     {
       NetworkPlayerHandler* player = i.next();
       if(room == player->getRoom())
         p.push_back(player->getPlayer());
     }
     return p;
   }

   void NetworkServer::addRoom(const QString& name, const QString& password, const QString& dmpassword, const QString &roomPropertiesFilepath)
   {
     Room* room = new Room(roomCtr,name,password,dmpassword, this, roomPropertiesFilepath);
     connect(room,&Room::sendMessage,this,&NetworkServer::sendAll);
     rooms.insert(roomCtr,room);
     roomCtr++;
   }

   void NetworkServer::acceptConnection()
   {
     while(server->hasPendingConnections())
     {
       QSslSocket* socket = server->nextPendingSslConnection();
       NetworkPlayerHandler* player = new NetworkPlayerHandler(socket,0,this);
       connect(player,&NetworkPlayerHandler::disconnected,this,&NetworkServer::deletePlayer);
       connect(player,&NetworkPlayerHandler::messageReceived,this,&NetworkServer::handleMessage);
       connect(player,&NetworkPlayerHandler::requestID,this,&NetworkServer::giveID);
       players.insert(player);
     }
   }

   void NetworkServer::deletePlayer(NetworkPlayerHandler *player)
   {
     players.remove(player);
     removePlayer(player,player->room);
     broadcastRooms();
     disconnect(player,&NetworkPlayerHandler::disconnected,this,&NetworkServer::deletePlayer);
     disconnect(player,&NetworkPlayerHandler::messageReceived,this,&NetworkServer::handleMessage);
     disconnect(player,&NetworkPlayerHandler::requestID,this,&NetworkServer::giveID);
     player->deleteLater();
     if(players.size()==0)
       idctr = 1;
     if(rooms.size()==0)
       roomCtr = 0;
   }

   void NetworkServer::sendAll(const orpg::Message& msg)
   {
      foreach (NetworkPlayerHandler* player, players)
      {
        player->send(msg);
      }
   }

   void NetworkServer::handleMessage(NetworkPlayerHandler *handler, orpg::Message msg)
   {
     if(msg.isValid())
     {
       if((handler->role==ROLE_NOINTRODUCTION) && (msg.type()!=BaseMessage::TypePlayer))
         return;
       switch(msg.type())
       {
         case BaseMessage::TypeChatMessage:
           handleMessage_chat(msg.toMessage<ChatMessage>(),*handler);
           break;
         case BaseMessage::TypePlayer:
           handleMessage_player(msg.toMessage<PlayerMessage>(),*handler);
           break;
         case BaseMessage::TypeAssignRole:
           handleMessage_assignStatus(msg.toMessage<AssignRoleMessage>(),*handler);
           break;
         case BaseMessage::TypeCreateRoom:
           handleMessage_createRoom(msg.toMessage<CreateRoomMessage>(),*handler);
           break;
         case BaseMessage::TypeJoinRoom:
           handleMessage_joinRoom(msg.toMessage<JoinRoomMessage>(),*handler);
           break;
         case BaseMessage::TypeKick:
           handleMessage_kick(msg.toMessage<KickMessage>(),*handler);
           break;
         case BaseMessage::TypeRoomResource:
           handleMessage_roomResource(msg.toMessage<RoomResourceMessage>(),*handler);
           break;
         case BaseMessage::TypeRoomVariable:
           handleMessage_roomVariable(msg.toMessage<RoomVariableMessage>(),*handler);
           break;
         case BaseMessage::TypeHeartbeat:
           handleMessage_heartbeat(msg.toMessage<HeartbeatMessage>(),*handler);
           break;
         default:
           QtIO::err << "Received unregistered message type." << endl;
           break;
       }
     }
     else
     {
       QtIO::err << "Received invalid message." << endl;
       handler->disconnectSocket();
     }
   }

   bool NetworkServer::openServer()
   {
     server->setPrivateKey(settings->privateKey);
     server->setCertificateChain(settings->certificateChain);
     bool success = server->listen(QHostAddress::Any,settings->port);
     emit startStatus(success);
     init();
     if(success && settings->notifyMetaServer)
       metaServerTimerTimeout();
     emit roomsChanged(rooms);
     return success;
   }

   void NetworkServer::closeServer()
   {
     server->close();
     metaServerTimer->stop();
     foreach (NetworkPlayerHandler* player, players)
     {
       player->disconnectSocket();
       delete player;
     }
     players.clear();
     foreach(Room* room, rooms.values())
       delete room;
     rooms.clear();
   }

   void NetworkServer::setSettings(const ServerSettings& settings)
   {
     this->settings->set(settings);
   }

   void NetworkServer::metaServerTimerTimeout()
   {
     metaServerClient->notifyHttpMetaServer(settings->serverName,settings->serverUrl,settings->port,players.size());
     metaServerTimer->start(settings->metaServerHeartbeat);
   }

   void NetworkServer::giveID()
   {
     NetworkPlayerHandler* handler = (NetworkPlayerHandler*) QObject::sender();
     handler->id = idctr;
     idctr++;
   }
}
