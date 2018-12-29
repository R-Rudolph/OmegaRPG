#include "roomlistwidget.h"

void RoomListWidget::enterRoom(RoomInfo room)
{
  if(room.hasPassword())
  {
    bool ok;
    QString pass = QInputDialog::getText(this, "Room Password",
                                         "Please enter the room password:", QLineEdit::Normal,
                                         "", &ok);
    if(ok && !pass.isEmpty())
      emit joinRoom(orpg::JoinRoomMessage(room.id(),pass));
  }
  else
  {
    emit joinRoom(orpg::JoinRoomMessage(room.id()));
  }
}

RoomListWidget::RoomListWidget(QWidget *parent) : QWidget(parent)
{
  vbox = new QVBoxLayout(this);
  roomLabel = new QLabel("Rooms:",this);
  roomTree = new QTreeWidget(this);
  joinButton = new QPushButton("Join Room",this);
  vbox->addWidget(roomLabel);
  vbox->addWidget(roomTree);
  vbox->addWidget(joinButton);

  roomTree->setColumnCount(2);
  roomTree->setColumnWidth(0,200);
  roomTree->setHeaderLabels({"Name","Players","Created"});
  roomTree->setRootIsDecorated(false);

  connect(roomTree,&QTreeWidget::itemDoubleClicked,this,&RoomListWidget::roomClicked);
  connect(joinButton,&QAbstractButton::clicked,this,&RoomListWidget::joinClicked);
}

void RoomListWidget::setRooms(QVector<RoomInfo> rooms)
{
  roomTree->clear();
  roomMap.clear();
  for(int i=0;i<rooms.size();i++)
  {
    QTreeWidgetItem* item = new QTreeWidgetItem(roomTree,
                                                {
                                                  rooms[i].name(),
                                                  QString::number(rooms[i].numPlayers()),
                                                  rooms[i].time().toString(Qt::ISODate)
                                                });
    QBrush brush;
    if(rooms[i].hasPassword())
      brush.setColor(QColor(255,0,0));
    item->setForeground(0,brush);
    item->setForeground(1,brush);
    roomMap.insert(item,rooms[i]);
  }
}

void RoomListWidget::roomClicked(QTreeWidgetItem* item,int)
{
  enterRoom(roomMap[item]);
}

void RoomListWidget::joinClicked()
{
  if(roomTree->currentItem())
  {
    enterRoom(roomMap[roomTree->currentItem()]);
  }
}
