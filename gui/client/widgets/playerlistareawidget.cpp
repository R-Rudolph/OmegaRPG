#include "playerlistareawidget.h"

PlayerListAreaWidget::PlayerListAreaWidget(QWidget *parent) : QWidget(parent)
{
  layout = new QStackedLayout(this);
  layout->setMargin(0);
  splitter = new QSplitter(Qt::Vertical,this);
  splitter->setChildrenCollapsible(false);
  playerList = new PlayerList(splitter);
  aliasSheet = new AliasSheetReadWidget(splitter);
  layout->addWidget(splitter);
  splitter->setSizes(QList<int>({1,1}));
  setAliasSheetVisible(false);

  connect(playerList,&PlayerList::bootFromRoom,this,&PlayerListAreaWidget::bootFromRoom);
  connect(playerList,&PlayerList::assignStatus,this,&PlayerListAreaWidget::assignStatus);
  connect(playerList,&PlayerList::openWhisper,this,&PlayerListAreaWidget::openWhisper);
  connect(aliasSheet,&AliasSheetReadWidget::chat,this,&PlayerListAreaWidget::chat);
}

void PlayerListAreaWidget::setID(quint32 id)
{
  playerList->setID(id);
}

void PlayerListAreaWidget::setClientStatus(PlayerRole status)
{
  playerList->setClientStatus(status);
}

void PlayerListAreaWidget::setPlayers(QVector<Player> players)
{
  playerList->setPlayers(players);
}

void PlayerListAreaWidget::setAliasSheetVisible(bool value)
{
  if(value)
  {
    aliasSheet->show();
  }
  else
  {
    aliasSheet->hide();
  }
}
