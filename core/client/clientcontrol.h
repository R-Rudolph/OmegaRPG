#ifndef CLIENTCONTROL_H
#define CLIENTCONTROL_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QTimer>
#include <QMessageBox>
#include <QMainWindow>
#include "clientcore.h"
#include "core/client/alias/aliaslib.h"
#include "core/client/util/sslsettings.h"
#include "settings.h"
#include "core/multi/util/global.h"
#include "dialogs/certificatedialog.h"
#include "core/multi/util/roominfo.h"
#include "core/multi/message/joinroommessage.h"

namespace orpg
{

class ClientControl : public QObject
{
  Q_OBJECT
  QString settingsfile;
  QTimer* typingTimer;
  QTimer* typingStoppedTimer;
public:
  ClientCore* core;
  Settings* settings;
  explicit ClientControl(QObject *parent = 0);
  const Settings* getSettings();

signals:
  void idChanged(quint32);
  void statusMessage(QString);
  void errorMessage(QString);
  void receivedChat(const orpg::ChatMessage& message);
  void publishSettings(const orpg::Settings&);
  void playersChanged(QVector<Player> players);

  void roomsChanged(QVector<RoomInfo>);
  void roleChanged(PlayerRole);

  void connected();
  void disconnected();
public slots:
  void sendMessage(orpg::Message);
  void chat(QString text, QString to);
  void connect(QString address);
  void typing();
  void stoppedTyping();
  void idle();
  void disconnectFromHost();

  void joinRoom(orpg::JoinRoomMessage room);
  void createRoom(QString name, QString pass, QString dmpass);
  void assignStatus(quint32 id, PlayerRole status, QString dmpass);
  void kick(quint32 id);
  void announceCurrentState();
private slots:
  void resendStatus();
  void sslErrors(const QList<QSslError> &errors);
};

}

#endif // CLIENTCONTROL_H
