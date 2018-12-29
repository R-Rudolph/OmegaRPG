#include "serverselectwidget.h"
#include<QSizePolicy>

QPushButton* ServerSelectWidget::disconnectButton() const
{
    return _disconnectButton;
}

ServerSelectWidget::ServerSelectWidget(QWidget *parent) : QWidget(parent)
{
    layout = new QGridLayout(this);

  addressLine = new QLineEdit(this);
  connectButton = new QPushButton("Connect");
  _disconnectButton = new QPushButton("Disconnect");
  refreshButton = new QPushButton(GlobalGUI::refreshIcon(),"Refresh");
  serverButton_ = new QPushButton(GlobalGUI::serverIcon(),"",this);
  serverTree = new QTreeWidget(this);

  serverButton_->setSizePolicy(QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed));
  serverButton_->setToolTip("Create Server");

  layout->addWidget(addressLine,0,0,1,4);
  layout->addWidget(serverTree,1,0,1,4);
  layout->addWidget(connectButton,2,0,1,1);
  layout->addWidget(_disconnectButton,2,1,1,1);
  layout->addWidget(refreshButton,2,2,1,1);
  layout->addWidget(serverButton_,2,3,1,1);

  serverTree->setColumnCount(3);
  serverTree->hideColumn(1);
  serverTree->setColumnWidth(0,200);
  serverTree->setRootIsDecorated(true);
  serverTree->setHeaderLabels({"Server","Address","Players"});
  serverTree->setContextMenuPolicy(Qt::CustomContextMenu);
  serverTree->setAcceptDrops(true);
  serverTree->setDragEnabled(true);
  serverTree->setDragDropMode(QAbstractItemView::InternalMove);
  serverTree->invisibleRootItem()->setFlags(serverTree->invisibleRootItem()->flags() & (~Qt::ItemIsDropEnabled));

  //add elements
  meta = new MetaServerSource(this);
  fav = new FavouritesSource(this);
  hist = new HistorySource(this);
  serverTree->addTopLevelItem(meta->getTreeWidgetItem());
  serverTree->addTopLevelItem(fav->getTreeWidgetItem());
  serverTree->addTopLevelItem(hist->getTreeWidgetItem());
  fav->getTreeWidgetItem()->setExpanded(true);
  meta->getTreeWidgetItem()->setExpanded(true);
  sources.insert(meta);
  sources.insert(fav);
  sources.insert(hist);

  connect(serverTree,&QTreeWidget::currentItemChanged,this,&ServerSelectWidget::selectServer);
  connect(serverTree,&QAbstractItemView::doubleClicked,this,&ServerSelectWidget::connectAddr);
  connect(connectButton,&QAbstractButton::clicked,this,&ServerSelectWidget::connectAddr);
  connect(_disconnectButton,&QAbstractButton::clicked,this,&ServerSelectWidget::disconnectFromHost);
  connect(refreshButton,&QAbstractButton::clicked,meta,&MetaServerSource::refresh);
  connect(serverTree,&QWidget::customContextMenuRequested,this,&ServerSelectWidget::contextMenu);
  connect(hist,&HistorySource::newFavourite,fav,&FavouritesSource::newFavourite);
  connect(meta,&MetaServerSource::newFavourite,fav,&FavouritesSource::newFavourite);
  connect(serverButton_,&QPushButton::clicked,this,&ServerSelectWidget::serverMenu);
}

void ServerSelectWidget::save()
{
}

void ServerSelectWidget::selectServer(QTreeWidgetItem *item, QTreeWidgetItem*)
{
  addressLine->setText(item->text(1));
}

void ServerSelectWidget::connectAddr()
{
  if(!addressLine->text().isEmpty())
  {
    if((serverTree->currentItem()!=nullptr) && (addressLine->text()==serverTree->currentItem()->text(1)))
    {
      hist->addEntry(serverTree->currentItem()->text(0),addressLine->text());
    }
    else
    {
      hist->addEntry(addressLine->text(),addressLine->text());
    }
    emit connectToHost(addressLine->text());
  }
}

void ServerSelectWidget::contextMenu(QPoint point)
{
  QTreeWidgetItem* item = serverTree->itemAt(point);
  if(item==nullptr)
    return;
  foreach(ServerSource* source, sources)
  {
    source->execContextMenu(item,serverTree->viewport()->mapToGlobal(point));
  }
}
