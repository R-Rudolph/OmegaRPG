#include "favouritessource.h"

bool FavouritesSource::load(const QString &filename)
{
  QString fname = filename;
  if(fname.isEmpty())
    fname = Global::favouritesFilepath();
  QFile file(fname);
  if(!file.open(QIODevice::ReadOnly))
    return false;
  QJsonParseError error;
  QJsonDocument doc = QJsonDocument::fromJson(file.readAll(),&error);
  file.close();
  if(error.error != QJsonParseError::NoError)
    return false;
  return parse(doc.object());
}

bool FavouritesSource::parse(const QJsonObject &favObject)
{
  if(favObject["type"].toString()!="OmegaRPG Server Favourites File")
    return false;
  list = ServerList(favObject["servers"].toArray());
  buildTree();
  return true;
}

void FavouritesSource::save(const QString &filename)
{
  QString fname = filename;
  if(fname.isEmpty())
    fname = Global::favouritesFilepath();
  QSaveFile file(fname);
  if(!file.open(QIODevice::WriteOnly))
    return;
  QJsonDocument doc(toJson());
  file.write(doc.toJson());
  file.commit();
}

void FavouritesSource::newFavourite(const QString &name, const QString &address)
{
  pushFavourite(address,name);
  save();
}

QJsonObject FavouritesSource::toJson()
{
  ServerList list;
  for(int i=0;i<rootItem->childCount();i++)
  {
    list.addEntry(ServerEntry(rootItem->child(i)->text(1),rootItem->child(i)->text(0)));
  }
  QJsonObject mainObject;
  mainObject.insert("type","OmegaRPG Server Favourites File");
  mainObject.insert("servers",list.toJson());
  return mainObject;
}

void FavouritesSource::buildTree()
{
  while(rootItem->childCount()>0)
    delete rootItem->child(0);
  for(int i=0;i<list.size();i++)
  {
    pushFavourite(list[i].address,list[i].name);
  }
}

FavouritesSource::FavouritesSource(QObject* parent) : ServerSource(parent)
{
  rootItem = new QTreeWidgetItem(QStringList({"Favorites",""}));
  QFont font = rootItem->font(0);
  font.setBold(true);
  rootItem->setFont(0,font);
  load();

  rootItem->setFlags(rootItem->flags() | (Qt::ItemIsDropEnabled));
}

void FavouritesSource::execContextMenu(QTreeWidgetItem *item, const QPoint &pos)
{
  if(item==rootItem)
  { //root item context menu
    QMenu menu;
    menu.addAction("New Favorite",this,&FavouritesSource::newEntryPressed);
    menu.exec(pos);
  }
  else if(item->parent()==rootItem)
  { //child item context menu
    currentItem = item;
    QMenu menu;
    menu.addAction("Edit",this,&FavouritesSource::editPressed);
    menu.addAction("Remove",this,&FavouritesSource::removePressed);
    menu.exec(pos);
  }
}

QTreeWidgetItem *FavouritesSource::getTreeWidgetItem()
{
  return rootItem;
}

void FavouritesSource::pushFavourite(const QString &address, const QString &name)
{
  QTreeWidgetItem* newItem = new QTreeWidgetItem(QStringList({name,address}));
  rootItem->addChild(newItem);
  newItem->setFlags(newItem->flags() & (~Qt::ItemIsDropEnabled));
}

void FavouritesSource::newEntryPressed()
{
  ServerEditDialog dialog;
  if(dialog.exec())
  {
    pushFavourite(dialog.getAddr(),dialog.getName());
    save();
  }
}

void FavouritesSource::editPressed()
{
  ServerEditDialog dialog;
  dialog.setName(currentItem->text(0));
  dialog.setAddr(currentItem->text(1));
  if(dialog.exec())
  {
    currentItem->setText(0,dialog.getName());
    currentItem->setText(1,dialog.getAddr());
    save();
  }
}

void FavouritesSource::removePressed()
{
  QMessageBox msgBox;
  msgBox.setText("Do you want to remove '"+currentItem->text(0)+"' from Favorites?");
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::Yes);
  if(msgBox.exec()==QMessageBox::Yes)
  {
    delete currentItem;
    save();
  }
}
