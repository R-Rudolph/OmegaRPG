#ifndef NETWORKPLAYERHANDLER_H
#define NETWORKPLAYERHANDLER_H

#include "core/server/networkhost.h"
#include "networkserver.h"

#include "core/multi/util/player.h"

namespace orpg
{

   class NetworkPlayerHandler : public QObject
   {
     Q_OBJECT
     static const QMap<BaseMessage::MessageType,PlayerRole> privilegeMap;
   public:
     int active;
     quint32 id;
     QString name;
     QString typingStatus;
     NetworkHost* client;
     NetworkServer* server;
     quint32 room;
     PlayerRole role;
     bool checkRights(BaseMessage::MessageType type);

     explicit NetworkPlayerHandler(QObject *parent = 0);
     explicit NetworkPlayerHandler(QSslSocket* socket, quint32 id, QObject *parent = 0);
     ~NetworkPlayerHandler();
     Player getPlayer();
     quint32 getRoom() const;

     Message getPlayerMessage() const;
     bool concerns(const Message& msg);
   signals:
      void messageReceived(NetworkPlayerHandler* handler, orpg::Message message);
      void sendMessageSignal(const orpg::Message& message);
      void disconnectSignal();
      void disconnected(NetworkPlayerHandler* player);
      void requestID();
   public slots:
      void disconnectedSlot();
      void disconnectSocket();
      void handleMessage(orpg::Message);
      void send(const orpg::Message&);
      void send(const QVector<orpg::Message>& messages);
      void send(const QList<orpg::Message>& messages);
   };

}

#endif // NETWORKPLAYERHANDLER_H
