#ifndef ROOMLISTWIDGET_H
#define ROOMLISTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QVector>
#include <QPushButton>
#include <QMap>
#include <QInputDialog>

#include "core/multi/message/joinroommessage.h"
#include "core/multi/util/roominfo.h"

class RoomListWidget : public QWidget
{
  Q_OBJECT
  QLabel* roomLabel;
  QTreeWidget* roomTree;
  QPushButton* joinButton;
  QVBoxLayout* vbox;
  QMap<QTreeWidgetItem*,RoomInfo> roomMap;

  void enterRoom(RoomInfo room);
public:
  explicit RoomListWidget(QWidget *parent = 0);

signals:
  void joinRoom(orpg::JoinRoomMessage room);
public slots:
  void setRooms(QVector<RoomInfo> rooms);
private slots:
  void roomClicked(QTreeWidgetItem* item,int);
  void joinClicked();
};

#endif // ROOMLISTWIDGET_H
