#ifndef CLIENTCORE_H
#define CLIENTCORE_H

#include <QVector>
#include <QTimer>
#include "networkclient.h"
#include "core/multi/message/message.h"
#include "core/multi/util/player.h"
#include "core/client/log/sessionmessagelogger2.h"
#include "core/client/log/jsonmessagelogger.h"
#include "core/client/log/openrpglogger.h"
#include "core/client/util/textformat.h"
#include "core/multi/util/roominfo.h"
#include "core/multi/message/joinroommessage.h"

#include "core/multi/roomresources/resourcecontainercollection.h"
#include "core/multi/roomvariables/roomvariablecontainercollection.h"

namespace orpg
{
  class ClientCore : public QObject
  {
    Q_OBJECT
    static ClientCore* staticCore;
    static const QSet<orpg::BaseMessage::MessageType> localLoopMessages;
    QString name;
    QString status;
    PlayerRole role;
    QVector<Player> players;
    QVector<RoomInfo> rooms;
    quint32 id;
    SessionMessageLogger2 sessionLogger;
    JsonMessageLogger jsonLogger;
    OpenRpgLogger openrpgLogger;
    QTimer* heartbeatTimer;
    ResourceContainerCollection* roomResources;
    RoomVariableContainerCollection* roomVariables;

    QString hostAddr;

    void handleMessage_chat(const orpg::ChatMessage& json);
    void handleMessage_setId(const orpg::SetIdMessage& json);
    void handleMessage_player(const orpg::PlayerMessage& json);
    void handleMessage_playerremove(const orpg::PlayerRemoveMessage& json);

    void handleMessage_giveRole(const orpg::GiveRoleMessage& json);
    void handleMessage_rooms(const orpg::RoomsMessage& json);
    void handleMessage_joinedRoom(const orpg::JoinedRoomMessage& json);
    void handleMessage_roomResource(const orpg::RoomResourceMessage& json);
    void handleMessage_roomVariable(const orpg::RoomVariableMessage& json);

    void setRole(PlayerRole role);
    Player* getPlayerById(const QString& id);
  public:
    static ClientCore* get();
    NetworkClient* client;
    explicit ClientCore(QObject *parent = 0);

    void kick(quint32 id);
    PlayerRole getRole() const;

    ResourceContainerCollection *getResources() const;
    RoomVariableContainerCollection *getVariables() const;

    void send(const Message& message);
    void send(const QVector<Message>& messages);
    void send(const QList<Message>& messages);

  signals:
    void idChanged(quint32);
    void errorMessage(QString);
    void statusMessage(QString);
    void receivedChat(const orpg::ChatMessage& message);
    void playersChanged(QVector<Player>);
    void roomsChanged(QVector<RoomInfo>);
    void sslErrors(const QList<QSslError>& errors);
    void initiativeChanged(const QList<InitiativeResource>& initiative);
    void roleChanged(PlayerRole role);
    void connectedSignal();
    void disconnectedSignal();
  private slots:
    void correctServer();
    void incorrectServer();
    void greetingTimeout();
  public slots:
    void sendClearMapMessage();
    bool exportRoomProperties(const QString& filepath);
    bool importRoomProperties(const QString& filepath);
    void activateNextInitiativeResource();
    void cloneInitiativeResource(int id);
    void moveInitiativeResource(int id, int position);
    void sortInitiativeResources();
    void newInitiativeResource(const InitiativeResource& resource);
    void setInitiativeResource(const InitiativeResource& resource);
    void setInitiativeResourceColor(int id, const QString& color);
    void removeInitiativeResource(int id);
    void activateInitiativeResource(int id);
    void reconnect();
    void connect(QString address);
    void disconnectFromHost();
    void handleMessage(orpg::Message msg);

    void setStatus(QString status);
    void setName(QString name);
    void chat(const QString& name, const QString& color, const QString& text, const QString& to=QString("all"));
    void player(const QString& status, const QString& name=QString());

    void connected();
    void encrypted();
    void disconnected();

    void sendingError();
    void socketError(QAbstractSocket::SocketError);

    void joinRoom(orpg::JoinRoomMessage room);
    void createRoom(QString name, QString pass, QString dmpass);
    void assignStatus(quint32 id, PlayerRole status, QString dmpass);
    void announceCurrentState();
  private slots:
    void heartbeat();
  };
}


#endif // CLIENTCORE_H
