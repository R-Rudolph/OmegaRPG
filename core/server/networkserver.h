#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QSet>
#include <QMap>
#include <QTimer>

#include "util/sslserver.h"
#include "util/sslhelper.h"
#include "core/multi/metaserverclient.h"
#include "serversettings.h"
#include "core/multi/message/message.h"
#include "core/multi/util/player.h"
#include "core/server/room.h"


namespace orpg
{

  class NetworkPlayerHandler;
  class NetworkServer;

  class NetworkServer : public QObject
  {
    Q_OBJECT

    static bool typesRegistered;
    static void registerTypes();

    MetaServerClient* metaServerClient;
    QTimer* metaServerTimer;
    SslServer* server;
    ServerSettings* settings;
    quint32 idctr;
    QSet<NetworkPlayerHandler*> players;
    QMap<int,Room*> rooms;
    int roomCtr=0;

    void handleMessage_player(PlayerMessage msg, NetworkPlayerHandler& handler);
    void handleMessage_chat(ChatMessage msg, NetworkPlayerHandler& handler);

    void handleMessage_assignStatus(AssignRoleMessage msg, NetworkPlayerHandler& handler);
    void handleMessage_createRoom(CreateRoomMessage msg, NetworkPlayerHandler& handler);
    void handleMessage_joinRoom(JoinRoomMessage msg, NetworkPlayerHandler& handler);
    void handleMessage_kick(KickMessage msg, NetworkPlayerHandler& handler);
    void handleMessage_roomResource(RoomResourceMessage msg, NetworkPlayerHandler& handler);
    void handleMessage_roomVariable(RoomVariableMessage msg, NetworkPlayerHandler& handler);
    void handleMessage_heartbeat(HeartbeatMessage msg, NetworkPlayerHandler& handler);

    //highlevel functions
    void enterRoom(NetworkPlayerHandler& handler, quint32 room, bool introduction);
    void assignStatus(NetworkPlayerHandler& handler, const PlayerRole& status);
    void createRoom(NetworkPlayerHandler& handler, QString name, QString password, QString dmpassword);
    void broadcastRooms();
    void removePlayer(NetworkPlayerHandler* player, quint32 room);
    void kick(NetworkPlayerHandler* kickedPlayer, NetworkPlayerHandler& kicker);

    void init();

    //helper functions
    orpg::Message serverChat(const QString& text, const QString& to, const QString& room);
    NetworkPlayerHandler* getPlayer(quint32 id);
  public:
    explicit NetworkServer(QObject *parent = 0, const ServerSettings& settings=ServerSettings());
    ~NetworkServer();
    QVector<Player> getPlayers(quint32 room);
    ServerSettings *getSettings() const;

  signals:
    void roomsChanged(const QMap<int,orpg::Room*> map);
    void startStatus(bool success);
  public slots:
    void addRoom(const QString& name, const QString& password, const QString& dmpassword, const QString& roomPropertiesFilepath=QString());
    void acceptConnection();
    void deletePlayer(NetworkPlayerHandler*);
    void sendAll(const orpg::Message& msg);
    void handleMessage(NetworkPlayerHandler* handler, orpg::Message msg);

    bool openServer();
    void closeServer();
    void setSettings(const orpg::ServerSettings&);
  private slots:
    void metaServerTimerTimeout();
    void giveID();
  };

}


#include "networkplayerhandler.h"

#endif // NETWORKSERVER_H
