#include "miniaturemenuwidget.h"
#include "core/client/clientcore.h"
#include "core/multi/roomresources/initiativeresource.h"
#include "core/multi/roomresources/mapresources/mapminiatureresource.h"

QJsonObject MiniatureMenuWidget::getJson(QTreeWidgetItem *item)
{
  if(isMiniature(item))
  {
    QJsonObject miniature;
    miniature.insert("type","miniature");
    miniature.insert("data",QJsonDocument::fromJson(item->text(2).toUtf8()).object());
    miniature.insert("comment",item->text(3));
    return miniature;
  }
  else
  {
    QJsonObject folder;
    folder.insert("type","folder");
    folder.insert("name",item->text(0));
    QJsonArray array;
    for(int i=0;i<item->childCount();i++)
      array.append(getJson(item->child(i)));
    folder.insert("folder",array);
    return folder;
  }
}

QTreeWidgetItem *MiniatureMenuWidget::getItemFromJson(const QJsonObject& json)
{
  QTreeWidgetItem* item = NULL;
  if(json["type"] == "miniature")
  {
    MapMiniatureResource resource = MapMiniatureResource(json["data"].toObject());
    item = getMiniatureItem(resource,json["comment"].toString(),&json);
  }
  else if(json["type"] == "folder")
  {
    item = getFolderItem(json["name"].toString());
    QJsonArray array = json["folder"].toArray();
    for(int i=0;i<array.size();i++)
    {
      QTreeWidgetItem* subItem = getItemFromJson(array.at(i).toObject());
      if(subItem)
        item->addChild(subItem);
    }
  }
  return item;
}

bool MiniatureMenuWidget::parseJson(const QJsonObject& json)
{
  if(json["type"].toString()=="OmegaRPG Miniature Library File")
  {
    QJsonArray array = json["folder"].toArray();
    for(int i=0;i<array.size();i++)
    {
      QTreeWidgetItem* item = getItemFromJson(array.at(i).toObject());
      if(item)
        tree->addTopLevelItem(item);
    }
    return true;
  }
  return false;
}

void MiniatureMenuWidget::load()
{
  tree->clear();
  load(Global::miniLibFilepath());
}

bool MiniatureMenuWidget::load(const QString &filename)
{
  QFile file;
  file.setFileName(filename);
  if(!file.open(QIODevice::ReadOnly))
  {
    return false;
  }
  QJsonParseError error;
  QJsonDocument doc = QJsonDocument::fromJson(file.readAll(),&error);
  file.close();
  if(error.error!=QJsonParseError::NoError)
  {
    return false;
  }
  QJsonObject json = doc.object();
  if(parseJson(json))
    return true;
  return false;
}

void MiniatureMenuWidget::save()
{
  save(Global::miniLibFilepath());
}

void MiniatureMenuWidget::save(const QString &filename)
{
  if(!pendingSave)
    return;
  QJsonObject json;
  QJsonArray array;
  json.insert("type","OmegaRPG Miniature Library File");
  for(int i=0;i<tree->topLevelItemCount();i++)
  {
    array.append(getJson(tree->topLevelItem(i)));
  }
  json.insert("folder",array);
  QJsonDocument document(json);
  QSaveFile file;
  file.setFileName(filename);
  if(!file.open(QIODevice::WriteOnly))
    return;
  file.write(document.toJson());
  file.commit();
  pendingSave = false;
}

void MiniatureMenuWidget::setData(QTreeWidgetItem *item, const MapMiniatureResource &miniature, const QString &comment)
{
  setResource(item,miniature);
  setComment(item,comment);
  if(comment.isEmpty())
    item->setText(0,miniature.getName());
  else
    item->setText(0,miniature.getName()+" ("+comment+")");
}

QString MiniatureMenuWidget::getComment(QTreeWidgetItem *item) const
{
  return item->text(3);
}

void MiniatureMenuWidget::setComment(QTreeWidgetItem *item, const QString &comment)
{
  item->setText(3,comment);
}

MapMiniatureResource MiniatureMenuWidget::getResource(QTreeWidgetItem *item) const
{
  return MapMiniatureResource(QJsonDocument::fromJson(item->text(2).toUtf8()).object());
}

void MiniatureMenuWidget::setResource(QTreeWidgetItem *item, const MapMiniatureResource &resource) const
{
  item->setText(2,QJsonDocument(resource.data()).toJson(QJsonDocument::Compact));
}

QTreeWidgetItem *MiniatureMenuWidget::getMiniatureItem(MapMiniatureResource miniature, const QString &comment, const QJsonObject* fallback)
{
  if(miniature.isValid())
  {
    if(fallback!=nullptr)
    {
      if(fallback->contains("name"))
        miniature.setName(fallback->value("name").toString());
      if(fallback->contains("display"))
        miniature.setDisplay((MapMiniatureResource::Display)fallback->value("display").toInt());
      if(fallback->contains("llayer"))
        miniature.setLayer((MapMiniatureResource::Layer)fallback->value("llayer").toInt());
      if(fallback->contains("size"))
        miniature.setSize(fallback->value("size").toDouble());
      if(fallback->contains("url"))
        miniature.setGraphic(fallback->value("url").toString());
      if(fallback->contains("vis"))
        miniature.setVisibility((MapMiniatureResource::Visibility)fallback->value("vis").toInt());
    }
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setFlags(item->flags() & (~Qt::ItemIsDropEnabled));
    setData(item,miniature,comment);
    setIsMiniature(item,true);
    return item;
  }
  else
  {
    return nullptr;
  }
}

QTreeWidgetItem *MiniatureMenuWidget::getFolderItem(const QString &name)
{
  QTreeWidgetItem* newFolder = new QTreeWidgetItem();
  newFolder->setFlags(newFolder->flags() | (Qt::ItemIsDropEnabled));
  QFont font = newFolder->font(0);
  font.setBold(true);
  newFolder->setFont(0,font);
  newFolder->setText(0,name);
  setIsMiniature(newFolder,false);
  return newFolder;
}

QTreeWidgetItem *MiniatureMenuWidget::getFolder(QTreeWidgetItem *item)
{
  if(item==NULL)
    return NULL;
  if(isMiniature(item))
    return item->parent();
  return item;
}

bool MiniatureMenuWidget::isMiniature(const QTreeWidgetItem *item) const
{
  if(item==NULL)
    return false;
  return item->text(1).toInt();
}

void MiniatureMenuWidget::setIsMiniature(QTreeWidgetItem *item, bool value)
{
  item->setText(1,QString::number(value));
}

MiniatureMenuWidget::MiniatureMenuWidget(QWidget *parent) : QWidget(parent)
{
  pendingSave = false;
  layout = new QVBoxLayout(this);
  layout->setMargin(0);
  tree = new QTreeWidget();
  label = new QLabel("Miniatures",this);

  tree->setHeaderLabel("Miniatures");
  tree->setColumnCount(6);
  tree->hideColumn(1);
  tree->hideColumn(2);
  tree->hideColumn(3);
  tree->hideColumn(4);
  tree->hideColumn(5);
  tree->hideColumn(6);
  tree->setSortingEnabled(true);
  tree->sortByColumn(1,Qt::AscendingOrder);
  tree->header()->close();
  tree->setAcceptDrops(true);
  tree->setDragEnabled(true);
  tree->setDragDropMode(QAbstractItemView::InternalMove);

  tree->setContextMenuPolicy(Qt::CustomContextMenu);

  layout->addWidget(label);
  layout->addWidget(tree);

  connect(tree,&QTreeWidget::currentItemChanged,this,&MiniatureMenuWidget::itemSelected);
  connect(tree,&QTreeWidget::itemDoubleClicked,this,&MiniatureMenuWidget::doubleClick);
  connect(tree,&QWidget::customContextMenuRequested,this,&MiniatureMenuWidget::contextMenu);

  load();
}

MiniatureMenuWidget::~MiniatureMenuWidget()
{
  save();
}

void MiniatureMenuWidget::itemSelected(QTreeWidgetItem* current,QTreeWidgetItem*)
{
  if(isMiniature(current))
  {
    miniature = getResource(current);
    emit previewData(miniature);
  }
}

void MiniatureMenuWidget::miniatureImport(const MapMiniatureResource& miniature)
{
  QTreeWidgetItem* folder = getFolder(tree->currentItem());
  if(folder==NULL)
  {
    for(int i=0;i<tree->topLevelItemCount();i++)
    {
      if(tree->topLevelItem(i)->text(0)=="Import")
      {
        folder = tree->topLevelItem(i);
        break;
      }
    }
    if(folder==NULL)
    {
      folder = getFolderItem("Import");
      tree->addTopLevelItem(folder);
    }
  }

  QTreeWidgetItem* newItem = getMiniatureItem(miniature,"Import");
  folder->addChild(newItem);
  folder->setExpanded(true);
  newItem->setSelected(true);
  tree->setCurrentItem(newItem);
  save();
}

void MiniatureMenuWidget::addMiniAction(bool)
{
  addMini(contextMenuItem);
}

void MiniatureMenuWidget::addFolderAction(bool)
{
  addFolder(contextMenuItem);
}

void MiniatureMenuWidget::addMini(QTreeWidgetItem *item)
{
  QTreeWidgetItem* folder = getFolder(item);
  MiniatureEditDialog dialog;
  dialog.setMiniatureMenuMode(true);
  if(dialog.exec())
  {
    QTreeWidgetItem* newItem = getMiniatureItem(dialog.getMiniature(),dialog.getComment());
    if(folder!=NULL)
    {
      folder->addChild(newItem);
      folder->setExpanded(true);
    }
    else
    {
      tree->addTopLevelItem(newItem);
    }
    newItem->setSelected(true);
    tree->setCurrentItem(newItem);
    pendingSave = true;
  }
}

void MiniatureMenuWidget::addFolder(QTreeWidgetItem *item)
{
  QTreeWidgetItem* folder = getFolder(item);
  bool ok;
  QString name = QInputDialog::getText(this,"Folder Name","Please enter the new folder name:",QLineEdit::Normal,"",&ok);
  if(ok)
  {
    QTreeWidgetItem* newFolder = getFolderItem(name);
    if(folder==NULL)
    {
      tree->addTopLevelItem(newFolder);
    }
    else
    {
      folder->addChild(newFolder);
      folder->setExpanded(true);
    }
    newFolder->setSelected(true);
    tree->setCurrentItem(newFolder);
    pendingSave = true;
  }
}

void MiniatureMenuWidget::addToInitiativeList()
{
  if(contextMenuItem==nullptr)
    return;
  MapMiniatureResource mini = getResource(contextMenuItem);
  InitiativeResource res(mini.getName(),
                         QString(),
                         mini.getGraphic(),
                         false,
                         0);
  orpg::ClientCore::get()->newInitiativeResource(res);
}

void MiniatureMenuWidget::contextMenu(QPoint point)
{
  QTreeWidgetItem* item = tree->itemAt(point);
  QMenu menu;
  contextMenuItem = item;
  menu.addAction(GlobalGUI::iconAdd(),"Add Miniature",this,&MiniatureMenuWidget::addMiniAction);
  menu.addAction(GlobalGUI::freedesktopNewFolder(),"Add Folder",this,&MiniatureMenuWidget::addFolderAction);
  menu.addSeparator();
  if(item!=NULL)
  {
    menu.addSeparator();
    if(isMiniature(item))
    {
      menu.addAction(GlobalGUI::freedesktopTextEditor(),"Edit",this,&MiniatureMenuWidget::editMiniatureMenu);
    }
    else
    {
      menu.addAction(GlobalGUI::freedesktopTextEditor(),"Edit",this,&MiniatureMenuWidget::editFolderMenu);
    }
    menu.addAction(GlobalGUI::freedesktopCopy(),"Copy",this,&MiniatureMenuWidget::cloneItem);
    menu.addAction(GlobalGUI::freedesktopRemove(),"Remove",this,&MiniatureMenuWidget::removeItem);
    menu.addSeparator();
  }
  menu.addAction(GlobalGUI::freedesktopSave(),"Export",this,&MiniatureMenuWidget::exportMinis);
  menu.addAction(GlobalGUI::freedesktopLoad(),"Import",this,&MiniatureMenuWidget::importMinis);
  menu.addSeparator();
  QAction* initiativeAction = menu.addAction("Add to Initiative List",this,&MiniatureMenuWidget::addToInitiativeList);
  if(!isMiniature(item))
    initiativeAction->setDisabled(true);
  menu.exec(tree->viewport()->mapToGlobal(point));
}

void MiniatureMenuWidget::doubleClick(QTreeWidgetItem* item, int)
{
  if(item!=NULL)
  {
    contextMenuItem = item;
    if(isMiniature(item))
    {
      editMiniatureMenu();
    }
    else
    {
      editFolderMenu();
    }
  }
}

void MiniatureMenuWidget::editMiniatureMenu()
{
  if(contextMenuItem==nullptr)
    return;
  MiniatureEditDialog dialog;
  dialog.setMiniatureMenuMode(true);
  dialog.setMiniature(getResource(contextMenuItem));
  dialog.setComment(getComment(contextMenuItem));
  if(dialog.exec())
  {
    setData(contextMenuItem,dialog.getMiniature(),dialog.getComment());
    tree->setCurrentItem(contextMenuItem);
    itemSelected(contextMenuItem,contextMenuItem);
    pendingSave = true;
  }
}

void MiniatureMenuWidget::editFolderMenu()
{
  bool ok;
  QString name = QInputDialog::getText(this,"Folder Name","Please enter the new folder name:",QLineEdit::Normal,contextMenuItem->text(0),&ok);
  if(ok)
  {
    contextMenuItem->setText(0,name);
    pendingSave = true;
  }
}

void MiniatureMenuWidget::removeItem()
{
  if(isMiniature(contextMenuItem))
  {
    delete contextMenuItem;
    pendingSave = true;
  }
  else
  {
    if(contextMenuItem->childCount()<=1)
    {
      delete contextMenuItem;
      pendingSave = true;
    }
    else
    {
      QMessageBox msgBox;
      msgBox.setText("Are you sure?");
      msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      msgBox.setDefaultButton(QMessageBox::Yes);
      if(msgBox.exec() == QMessageBox::Yes)
      {
        delete contextMenuItem;
        pendingSave = true;
      }
    }
  }
}

void MiniatureMenuWidget::cloneItem()
{
  QTreeWidgetItem* clonedItem = contextMenuItem->clone();

  QString comment = getComment(clonedItem);
  if(comment.isEmpty())
    comment = "Copy";
  else if (!comment.endsWith("Copy"))
    comment += ", Copy";
  setData(clonedItem,getResource(clonedItem),comment);
  int index;
  if(contextMenuItem->parent()==NULL)
  {
    index = tree->indexOfTopLevelItem(contextMenuItem);
    tree->insertTopLevelItem(index+1,clonedItem);
  }
  else
  {
    index = contextMenuItem->parent()->indexOfChild(contextMenuItem);
    contextMenuItem->parent()->insertChild(index+1,clonedItem);
  }
  pendingSave = true;
}

void MiniatureMenuWidget::exportMinis()
{
  QString filename = QFileDialog::getSaveFileName(this,"Export Miniatures","./miniatures.json","JSON file (*.json);;All files (*)");
  if(!filename.isEmpty())
  {
    save(filename);
  }
}

void MiniatureMenuWidget::importMinis()
{
  QString filename = QFileDialog::getOpenFileName(this,"Import Miniatures",".","JSON file (*.json);;All files (*)");
  if(!filename.isEmpty())
  {
    if(!load(filename))
    {
      QMessageBox::warning(this,"Import Error","Could not import.",QMessageBox::Ok);
    }
    else
      pendingSave = true;
  }
}

void MiniatureMenuWidget::hide()
{
  save();
  QWidget::hide();
}
