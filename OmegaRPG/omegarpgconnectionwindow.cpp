#include "omegarpgconnectionwindow.h"
#include "objects/nightmode.h"

OmegaRpgConnectionWindow::OmegaRpgConnectionWindow(QWidget *parent) : NightModeOverlay(parent)
{
  this->resize(600,400);

  this->setWidget(new QWidget(this));
  layout = new QGridLayout(this->getWidget());
  serverSelect = new ServerSelectWidget(this);
  roomList = new RoomListWidget(this);
  roomCreation = new RoomCreationWidget(this);
  closeButton = new QPushButton("Close",this);

  layout->addWidget(serverSelect,0,0,2,1);
  layout->addWidget(roomList,0,1);
  layout->addWidget(roomCreation,1,1);
  layout->addWidget(closeButton,2,0,1,2);

  //signals
  connect(closeButton,&QAbstractButton::clicked,this,&QWidget::close);
  //server select signals
  connect(serverSelect,&ServerSelectWidget::connectToHost,this,&OmegaRpgConnectionWindow::connectToHost);
  connect(serverSelect,&ServerSelectWidget::disconnectFromHost,this,&OmegaRpgConnectionWindow::disconnectFromHost);
  connect(serverSelect,&ServerSelectWidget::refresh,this,&OmegaRpgConnectionWindow::refreshServerList);
  connect(serverSelect,&ServerSelectWidget::serverMenu,this,&OmegaRpgConnectionWindow::serverMenu);
  //room list signals
  connect(roomList,&RoomListWidget::joinRoom,this,&OmegaRpgConnectionWindow::joinRoom);
  //room creation signals
  connect(roomCreation,&RoomCreationWidget::createRoom,this,&OmegaRpgConnectionWindow::createRoom);
  disconnected();
}

void OmegaRpgConnectionWindow::closeEvent(QCloseEvent *event)
{
  lastSize = QWidget::size();
  lastPosition = QWidget::pos();
  serverSelect->save();
  event->accept();
}

bool OmegaRpgConnectionWindow::restoreGeometry(const QByteArray &geometry)
{
  bool result = QWidget::restoreGeometry(geometry);
  return result;
}

void OmegaRpgConnectionWindow::show()
{
  QWidget::show();
  if(lastSize.isValid())
  {
    resize(QSize(0,0));
    move(lastPosition);
    resize(lastSize);
  }
}

void OmegaRpgConnectionWindow::setRooms(QVector<RoomInfo> rooms)
{
  roomList->setRooms(rooms);
}

void OmegaRpgConnectionWindow::connected()
{
  roomList->setDisabled(false);
  roomCreation->setDisabled(false);
  serverSelect->disconnectButton()->setDisabled(false);
}

void OmegaRpgConnectionWindow::disconnected()
{
  roomList->setDisabled(true);
  roomCreation->setDisabled(true);
  serverSelect->disconnectButton()->setDisabled(true);
}
