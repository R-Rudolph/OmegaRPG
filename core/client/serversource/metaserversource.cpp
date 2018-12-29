#include "metaserversource.h"

void MetaServerSource::clearAllChildren()
{
  while(rootItem->childCount()>0)
    delete rootItem->child(0);
}

QTreeWidgetItem *MetaServerSource::itemFromServer(const ServerEntry &server)
{
  QTreeWidgetItem* item = new QTreeWidgetItem(QStringList({server.name,server.address,QString::number(server.players)}));
  item->setFlags(item->flags() & (~Qt::ItemIsDropEnabled));
  return item;
}

MetaServerSource::MetaServerSource(QObject *parent) : ServerSource(parent)
{
  error = false;
  metaServerClient = new MetaServerClient;
  metaServerClient->setHttpMetaServerUrls(Global::getStaticMetaServerUrls());
  rootItem = new QTreeWidgetItem(QStringList({"Servers"}));
  brush = rootItem->foreground(0);
  QFont font = rootItem->font(0);
  font.setBold(true);
  rootItem->setFont(0,font);
  rootItem->setFlags(rootItem->flags() & (~Qt::ItemIsDropEnabled));
  connect(metaServerClient,&MetaServerClient::getSuccessful,this,&MetaServerSource::setServerList);
  connect(metaServerClient,&MetaServerClient::getFailed,this,&MetaServerSource::getFailed);
  connect(metaServerClient,&MetaServerClient::statusChanged,this,&MetaServerSource::statusChanged);
  refresh();
}

void MetaServerSource::execContextMenu(QTreeWidgetItem *item, const QPoint &pos)
{
  if(item==nullptr)
    return;
  if(item==rootItem)
  { //root contex menu
    QMenu menu;
    menu.addAction("Refresh",this,&MetaServerSource::refresh);
    if(!errorMsg.isEmpty())
      menu.addAction("Status",this,&MetaServerSource::status);
    menu.exec(pos);
  }
  else if(item->parent()==rootItem)
  { //child contex menu
    currentItem = item;
    QMenu menu;
    menu.addAction("Refresh",this,&MetaServerSource::refresh);
    menu.addAction("Add to favorites",this,&MetaServerSource::addToFavoritesPressed);
    menu.exec(pos);
  }
}

QTreeWidgetItem *MetaServerSource::getTreeWidgetItem()
{
  return rootItem;
}

void MetaServerSource::refresh()
{
  metaServerClient->setHttpMetaServerUrls(orpg::Settings::get()->getMetaServerUrls());
  metaServerClient->getServersHttpMetaServer();
}

void MetaServerSource::status()
{
  QMessageBox::information(nullptr,"OmegaRPG",errorMsg,QMessageBox::Ok,QMessageBox::Ok);
}

void MetaServerSource::setServerList(ServerList list)
{
  clearAllChildren();
  for(int i=0;i<list.size();i++)
  {
    rootItem->addChild(itemFromServer(list[i]));
  }
}

void MetaServerSource::getFailed(QNetworkReply::NetworkError, QString)
{
}

void MetaServerSource::addToFavoritesPressed()
{
  emit newFavourite(currentItem->text(0),currentItem->text(1));
}

void MetaServerSource::statusChanged(QMap<QString, MetaServerClient::MetaServerStatus> map)
{
  errorMsg.clear();
  bool loading = false;
  bool everythingError = true;
  for(QMap<QString, MetaServerClient::MetaServerStatus>::iterator i = map.begin(); i!=map.end(); i++)
  {
    errorMsg.append(i.key()+": "+MetaServerClient::metaServerStatusString(i.value())+"\n");
    if(i.value()==MetaServerClient::STATUS_OK)
      everythingError = false;
    if(i.value()==MetaServerClient::STATUS_NONE)
      loading = true;
  }
  errorMsg.chop(1);
  if(loading)
  {
    rootItem->setForeground(0,QBrush(Qt::blue));
  }
  else if(everythingError)
  {
    rootItem->setForeground(0,QBrush(Qt::red));
  }
  else
  {
    rootItem->setData(0, Qt::ForegroundRole, QVariant());
  }
}
