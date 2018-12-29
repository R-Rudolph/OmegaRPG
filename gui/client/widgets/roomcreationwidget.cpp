#include "roomcreationwidget.h"
#include<QRandomGenerator>

QString RoomCreationWidget::randPass(int length) const
{
  const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
  QString pass;
  for(int i=0;i<length;i++)
  {
    pass.append(possibleCharacters.at(QRandomGenerator::global()->generate()%possibleCharacters.length()));
  }
  return pass;
}

RoomCreationWidget::RoomCreationWidget(QWidget *parent) : QWidget(parent)
{
  //set up layout stuff
  vbox = new QVBoxLayout(this);
  gridWidget = new QWidget();
  vbox->addWidget(gridWidget);
  grid = new QGridLayout(gridWidget);
  //init widgets
  nameLabel = new QLabel("Room Name:");
  nameLabel->setToolTip("The name of the Room. This is shown in the room list.");
  passLabel = new QLabel("Password:");
  passLabel->setToolTip("The password of the room. This is required to enter the room. Leave empty to allow everyone in.");
  gmPassLabel = new QLabel("GM Password:");
  gmPassLabel->setToolTip("The GM password for the room. This is required to assume the role of DM. If left empty, a random password will be generated.");

  nameEdit = new QLineEdit();
  nameEdit->setToolTip("The name of the Room. This is shown in the room list.");
  passEdit = new QLineEdit();
  passEdit->setToolTip("The password of the room. This is required to enter the room. Leave empty to allow everyone in.");
  passEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
  gmPassEdit = new QLineEdit();
  gmPassEdit->setToolTip("The GM password for the room. This is required to assume the role of DM. If left empty, a random password will be generated.");
  gmPassEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);

  createButton = new QPushButton("Create Room");
  //add widgets to layouts
  vbox->addWidget(createButton);

  grid->addWidget(nameLabel,0,0,Qt::AlignRight);
  grid->addWidget(passLabel,1,0,Qt::AlignRight);
  grid->addWidget(gmPassLabel,2,0,Qt::AlignRight);

  grid->addWidget(nameEdit,0,1,Qt::AlignLeft);
  grid->addWidget(passEdit,1,1,Qt::AlignLeft);
  grid->addWidget(gmPassEdit,2,1,Qt::AlignLeft);

  //signal handling
  connect(createButton,&QAbstractButton::clicked,this,&RoomCreationWidget::createButtonPressed);
  connect(nameEdit,&QLineEdit::returnPressed,this,&RoomCreationWidget::createButtonPressed);
  connect(passEdit,&QLineEdit::returnPressed,this,&RoomCreationWidget::createButtonPressed);
  connect(gmPassEdit,&QLineEdit::returnPressed,this,&RoomCreationWidget::createButtonPressed);
}

void RoomCreationWidget::createButtonPressed()
{
  QString pass = gmPassEdit->text();
  if(pass.isEmpty())
    pass = randPass(20);
  gmPassEdit->setText(pass);
  emit createRoom(nameEdit->text(),passEdit->text(),pass);
}
