#include "playerlist.h"


void PlayerList::assignStatusFunc(PlayerRole status)
{
  if(this->status == ROLE_GM)
  {
    emit assignStatus(selectedPlayer,status,"");
  }
  else
  {
    bool ok;
    QString newPass = QInputDialog::getText(this,"GM Password","Please enter the DM password for this room.",QLineEdit::Password,gmpass,&ok);
    if(ok)
    {
      gmpass = newPass;
      emit assignStatus(selectedPlayer,status,gmpass);
    }
  }
}

PlayerList::PlayerList(QWidget *parent) : QWidget(parent)
{
  layout = new QVBoxLayout(this);
  list = new QTreeWidget();
  list->setColumnCount(3);
  list->setHeaderLabels(QStringList({"ID","Name","Status"}));
  list->setColumnWidth(0,30);
  list->setColumnWidth(1,100);
  list->setColumnWidth(2,70);
  list->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  list->setContextMenuPolicy(Qt::CustomContextMenu);
  list->setSelectionMode(QAbstractItemView::NoSelection);
  list->setFocusPolicy(Qt::NoFocus);
  list->setRootIsDecorated(false);
  layout->addWidget(list);
  layout->setMargin(0);
  connect(list,&QWidget::customContextMenuRequested,this,&PlayerList::contextMenu);
  connect(list,&QTreeWidget::itemDoubleClicked,this,&PlayerList::whisperSlotDoubleClick);
}

void PlayerList::setID(quint32 id)
{
  this->id = id;
}

void PlayerList::setClientStatus(PlayerRole status)
{
  this->status = status;
}

void PlayerList::setPlayers(QVector<Player> players)
{
  list->clear();
  for(int i=0;i<players.size();i++)
  {
    QTreeWidgetItem* newPlayer = new QTreeWidgetItem;
    newPlayer->setText(0,players[i].id);
    newPlayer->setText(1,players[i].name);
    newPlayer->setText(2,players[i].status);
    if(players[i].id == QString::number(id))
    {
      QFont font;
      font.setBold(true);
      newPlayer->setFont(0,font);
      newPlayer->setFont(1,font);
      newPlayer->setFont(2,font);
    }
    QColor color;
    switch(players[i].role)
    {
      case ROLE_LURKER:
        color = QColor(128,128,128);
        break;
      case ROLE_GM:
        color = QColor(255,0,0);
        break;
      default:
        color = QColor(0,0,0);
        break;
    }
    newPlayer->setForeground(0,QBrush(color));
    newPlayer->setForeground(1,QBrush(color));
    newPlayer->setForeground(2,QBrush(color));
    list->addTopLevelItem(newPlayer);
  }
}

void PlayerList::contextMenu(const QPoint &pos)
{
  QTreeWidgetItem* item = list->itemAt( pos );
  if(item==nullptr)
    return;
  bool ok;
  selectedPlayer = item->text(0).toUInt(&ok);
  selectedPlayerName = item->text(1);
  if(!ok)
    return;
  QMenu menu(this);
  menu.addAction(item->text(1));
  menu.addAction("Whisper",this,&PlayerList::whisperSlot);
  QAction* bootAction = menu.addAction("Kick",this,&PlayerList::bootSlot);
  menu.addSeparator();
  menu.addAction("Make Lurker",this,&PlayerList::lurkerSlot);
  menu.addAction("Make Player",this,&PlayerList::playerSlot);
  menu.addAction("Make GM",this,&PlayerList::gmSlot);
  menu.actions().at(0)->setDisabled(true);
  if(selectedPlayer==id)
  {
    menu.actions().at(1)->setDisabled(true);
  }
  if(status!=ROLE_GM)
  {
    bootAction->setDisabled(true);
  }
  menu.exec(list->viewport()->mapToGlobal(pos));
}

void PlayerList::whisperSlotDoubleClick(QTreeWidgetItem* item, int)
{
  bool ok;
  quint32 id = item->text(0).toUInt(&ok);
  if(ok)
  {
    emit openWhisper(id);
  }
}

void PlayerList::whisperSlot()
{
  emit openWhisper(selectedPlayer);
}

void PlayerList::bootSlot()
{
  if(id == selectedPlayer)
  {
    int answer = QMessageBox::question(this,
                                       "kik",
                                       "Really kick yourself? I mean, you could just leave if you wanted to...",
                                       QMessageBox::Yes,
                                       QMessageBox::No);
    if(answer!=QMessageBox::Yes)
      return;
  }
  emit bootFromRoom(selectedPlayer);
}

void PlayerList::lurkerSlot()
{
  assignStatusFunc(ROLE_LURKER);
}

void PlayerList::playerSlot()
{
  assignStatusFunc(ROLE_PLAYER);
}

void PlayerList::gmSlot()
{
  if(status==ROLE_GM)
  {
    QMessageBox msgBox(this);
    msgBox.setText("Are you sure you want to make "+selectedPlayerName+"("+QString::number(selectedPlayer)+") a GM?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Yes)
      assignStatusFunc(ROLE_GM);
  }
  else
  {
    assignStatusFunc(ROLE_GM);
  }
}
