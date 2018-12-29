#include "historysource.h"

void HistorySource::parse(const QJsonObject &histObj)
{
  if(histObj["type"].toString()!="OmegaRPG Server History File")
    return;
  ServerList list(histObj["servers"].toArray());
  for(int i=list.size()-1;i>=0;i--)
  {
    addEntryInternal(list[i].name,list[i].address);
  }
}

void HistorySource::load(const QString& filename)
{
  QString fname = filename;
  if(fname.isEmpty())
    fname = Global::historyFilepath();
  QFile file(fname);
  if(!file.open(QIODevice::ReadOnly))
    return;
  QJsonParseError error;
  QJsonDocument doc = QJsonDocument::fromJson(file.readAll(),&error);
  file.close();
  if(error.error != QJsonParseError::NoError)
    return;
  parse(doc.object());
}

void HistorySource::save(const QString& filename)
{
  QString fname = filename;
  if(fname.isEmpty())
    fname = Global::historyFilepath();
  QSaveFile file(fname);
  if(!file.open(QIODevice::WriteOnly))
    return;
  QJsonDocument doc(toJson());
  file.write(doc.toJson());
  file.commit();
}

QJsonObject HistorySource::toJson()
{
  ServerList list;
  for(int i=0;i<rootItem->childCount();i++)
  {
    list.addEntry(ServerEntry(rootItem->child(i)->text(1),rootItem->child(i)->text(0)));
  }
  QJsonObject mainObject;
  mainObject.insert("type","OmegaRPG Server History File");
  mainObject.insert("servers",list.toJson());
  return mainObject;
}

void HistorySource::addEntryInternal(const QString &name, const QString &address)
{
  QTreeWidgetItem* item = new QTreeWidgetItem(QStringList({name,address}));
  item->setFlags(rootItem->flags() & (~Qt::ItemIsDropEnabled));
  for(int i=0;i<rootItem->childCount();i++)
  {
    if(rootItem->child(i)->text(1)==address)
    {
      delete rootItem->child(i);
      i--;
    }
  }
  rootItem->insertChild(0,item);
  int num = orpg::Settings::get()->numServerHistory;
  while(rootItem->childCount()>num)
    delete rootItem->child(num);
}

HistorySource::HistorySource(QObject *parent) : ServerSource(parent)
{
  rootItem = new QTreeWidgetItem(QStringList({"History"}));
  QFont font = rootItem->font(0);
  font.setBold(true);
  rootItem->setFont(0,font);
  rootItem->setFlags(rootItem->flags() & (~Qt::ItemIsDropEnabled));
  load();
}

void HistorySource::execContextMenu(QTreeWidgetItem *item, const QPoint &pos)
{
  if(item==nullptr)
    return;
  if(item==rootItem)
  { //root item context menu
    QMenu menu;
    menu.addAction("Clear History",this,&HistorySource::clearHistoryPressed);
    menu.exec(pos);
  }
  else if(item->parent()==rootItem)
  { //child item contex menu
    currentItem = item;
    QMenu menu;
    menu.addAction("Add To Favorites",this,&HistorySource::addToFavouritesPressed);
    menu.addAction("Remove",this,&HistorySource::removeItemPressed);
    menu.exec(pos);
  }
}

QTreeWidgetItem *HistorySource::getTreeWidgetItem()
{
  return rootItem;
}

void HistorySource::addEntry(const QString &name, const QString &address)
{
  addEntryInternal(name,address);
  save();
}

void HistorySource::clearHistoryPressed()
{
  QMessageBox msgBox;
  msgBox.setText("Are you sure you want to clear your server history?");
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::Yes);
  if(msgBox.exec()==QMessageBox::Yes)
  {
    while(rootItem->childCount()>0)
      delete rootItem->child(0);
    save();
  }
}

void HistorySource::removeItemPressed()
{
  QMessageBox msgBox;
  msgBox.setText("Are you sure you want to remove this entry from the history?");
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::Yes);
  if(msgBox.exec()==QMessageBox::Yes)
  {
    delete currentItem;
    save();
  }
}

void HistorySource::addToFavouritesPressed()
{
  emit newFavourite(currentItem->text(0),currentItem->text(1));
}
