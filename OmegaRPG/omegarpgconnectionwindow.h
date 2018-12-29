#ifndef OMEGARPGCONNECTIONWINDOW_H
#define OMEGARPGCONNECTIONWINDOW_H

#include <QGridLayout>
#include <QPushButton>
#include <QCloseEvent>
#include "widgets/serverselectwidget.h"
#include "widgets/roomlistwidget.h"
#include "widgets/roomcreationwidget.h"
#include "widgets/nightmodeoverlay.h"
#include "core/multi/util/roominfo.h"
#include "core/multi/message/joinroommessage.h"

class OmegaRpgConnectionWindow : public NightModeOverlay
{
  Q_OBJECT
  QPoint lastPosition;
  QSize lastSize;
  QGridLayout* layout;
  ServerSelectWidget* serverSelect;
  RoomListWidget* roomList;
  RoomCreationWidget* roomCreation;
  QPushButton* closeButton;
public:
  explicit OmegaRpgConnectionWindow(QWidget *parent = 0);
  void closeEvent(QCloseEvent *event);
  bool restoreGeometry(const QByteArray &geometry);
signals:
  void connectToHost(QString);
  void disconnectFromHost();
  void serverMenu();
  void refreshServerList();
  void setServerList(ServerList serverList);

  void joinRoom(orpg::JoinRoomMessage room);
  void createRoom(QString name, QString pass, QString dmpass);
public slots:
  void show();
  void setRooms(QVector<RoomInfo> rooms);
  void connected();
  void disconnected();
};

#endif // OMEGARPGCONNECTIONWINDOW_H
