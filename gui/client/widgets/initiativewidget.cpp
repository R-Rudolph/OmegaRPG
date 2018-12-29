#include "initiativewidget.h"

#include "core/multi/roomresources/initiativeresource.h"
#include "core/multi/roomresources/initiativeresourcecontainer.h"
#include "core/client/clientcore.h"
#include "dialogs/initiativedialog.h"
#include "widgets/initiativetreewidget.h"
#include <QMenu>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>

#include "gui/util/globalgui.h"
#include "gui/client/objects/imageprovider.h"

void InitiativeWidget::addItemFromResource(const InitiativeResource &resource)
{
  QTreeWidgetItem* item = new QTreeWidgetItem(initiativeList,{QString(),
                                                              QString(),
                                                              QString::number(resource.getInitiative()),
                                                              resource.getName(),
                                                              resource.getStatus(),
                                                              QString::number(resource.getID()),
                                                              QString::number(resource.getPosition())});
  item->setFlags(item->flags() & (~Qt::ItemIsDropEnabled));
  QUrl url = QUrl(resource.getIconUrl());
  if(url.isValid())
  {
    initiativeList->setColumnHidden(1,false);
    item->setIcon(1,ImageProvider::get()->getIcon(url));
  }
  QBrush brush = QBrush(QColor(resource.getColor()));
  item->setForeground(2,brush);
  item->setForeground(3,brush);
  item->setForeground(4,brush);
  if(resource.getIsActive())
  {
    activeInitiative = item;
    QFont font = item->font(0);
    font.setBold(true);
    item->setFont(2,font);
    item->setFont(3,font);
    item->setFont(4,font);
  }
  itemIdMap.insert(item,resource.getID());
}

void InitiativeWidget::updateImage(const QUrl &url, bool failed)
{
  InitiativeResourceContainer* initiative = orpg::ClientCore::get()->getResources()->getInitiativeContainer();
  QIcon icon;
  if(!failed)
    icon = ImageProvider::get()->getIcon(url);
  else
    icon = ImageProvider::get()->getFailureIcon();
  for(auto it = itemIdMap.begin();it!=itemIdMap.end();++it)
  {
    QUrl itemUrl(((InitiativeResource*)initiative->getResourceById(it.value()))->getIconUrl());
    if(itemUrl==url)
    {
      it.key()->setIcon(1,icon);
    }
  }
}

void InitiativeWidget::setIconSize(int value)
{
  initiativeList->setIconSize(QSize(value,value));
}

QIcon InitiativeWidget::fromColor(const QColor &color)
{
  QPixmap pixmap(20,20);
  pixmap.fill(color);
  return QIcon(pixmap);
}

QAction* InitiativeWidget::colorAction(const QString &name, const QColor &color)
{
  QAction* action = new QAction(fromColor(color),name);
  action->setData(QVariant(color));
  connect(action,&QAction::triggered,this,&InitiativeWidget::setColor);
  return action;
}

void InitiativeWidget::fillColorMenu(QMenu *menu)
{
  menu->addAction(colorAction("",Qt::black));
  for(int i=0;i<360;i+=20)
  {
    menu->addAction(colorAction("",QColor::fromHsl(i,255,96)));
  }
}

InitiativeWidget::InitiativeWidget(QWidget *parent) : QWidget(parent)
{
  lastPosition = -1;
  initiativeDialog = new InitiativeDialog(this);
  layout = new QVBoxLayout(this);
  layout->setMargin(0);
  initiativeList = new InitiativeTreeWidget(this);
  nextButton = new QPushButton(GlobalGUI::freedesktopGoNext(),"Next",this);
  layout->addWidget(initiativeList);
  layout->addWidget(nextButton);

  initiativeList->setSortingEnabled(false);
  initiativeList->setHeaderLabels({"Unused","","Initiative","Name","Status","ID","Position"});
  initiativeList->setContextMenuPolicy(Qt::CustomContextMenu);
  initiativeList->setAcceptDrops(false);
  initiativeList->setDragEnabled(false);
  initiativeList->setDragDropMode(QAbstractItemView::InternalMove);
  //Column 0 is inserted and hidden in order to work around QTBUG-33974
  initiativeList->setColumnHidden(0,true);
  initiativeList->setColumnHidden(5,true);
  initiativeList->setColumnHidden(6,true);
  initiativeList->setFocusPolicy(Qt::NoFocus);
  initiativeList->header()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
  initiativeList->header()->setSectionsMovable(true);
  iconSizeMedium();
  initiativeList->setRootIsDecorated(false);

  connect(initiativeList,&QTreeWidget::customContextMenuRequested,this,&InitiativeWidget::initiativeListContextMenu);
  connect(orpg::ClientCore::get(),&orpg::ClientCore::initiativeChanged,this,&InitiativeWidget::setInitiative);
  connect(initiativeList,&InitiativeTreeWidget::itemDropped,this,&InitiativeWidget::moveItem);
  connect(nextButton,&QPushButton::clicked,this,&InitiativeWidget::nextInitiative);
  connect(initiativeList,&InitiativeTreeWidget::itemDoubleClicked,this,&InitiativeWidget::itemDoubleClicked);
  connect(orpg::ClientCore::get(),&orpg::ClientCore::roleChanged,this,&InitiativeWidget::roleChanged);

  connect(ImageProvider::get(),&ImageProvider::imageAvailable,this,&InitiativeWidget::imageComplete);
  connect(ImageProvider::get(),&ImageProvider::imageUnavaiable,this,&InitiativeWidget::imageFailed);
  roleChanged(ROLE_GM);
}

void InitiativeWidget::clone()
{
  if(itemIdMap.contains(initiativeContextMenuItem))
  {
    int id = itemIdMap[initiativeContextMenuItem];
    orpg::ClientCore::get()->cloneInitiativeResource(id);
  }
}

void InitiativeWidget::setInitiative(const QList<InitiativeResource> &initiative)
{
  initiativeContextMenuItem = nullptr;
  foreach(QTreeWidgetItem* item, itemIdMap.keys())
  {
    delete item;
  }
  itemIdMap.clear();
  initiativeList->setColumnHidden(1,true);
  foreach(InitiativeResource resource, initiative)
  {
    addItemFromResource(resource);
    lastPosition = resource.getPosition();
  }
  initiativeList->headerItem()->setText(2,"");
  initiativeList->resizeColumnToContents(2);
  initiativeList->headerItem()->setText(2,"Initiative");
}

void InitiativeWidget::initiativeListContextMenu(const QPoint &pos)
{
  if(orpg::ClientCore::get()->getRole()<ROLE_GM)
  {
    QMenu menu;
    QMenu* iconSize = menu.addMenu(GlobalGUI::freedesktopZoomFitBest(),"Icon Size");
    iconSize->addAction("Small",this,&InitiativeWidget::iconSizeSmall);
    iconSize->addAction("Medium",this,&InitiativeWidget::iconSizeMedium);
    iconSize->addAction("Large",this,&InitiativeWidget::iconSizeLarge);
    iconSize->addAction("Huge",this,&InitiativeWidget::iconSizeHuge);
    iconSize->addAction("Gargantuan",this,&InitiativeWidget::iconSizeGargantuan);
    menu.exec(initiativeList->viewport()->mapToGlobal(pos));
  }
  else
  {
    QMenu menu;
    initiativeContextMenuItem = initiativeList->itemAt(pos);
    if(initiativeContextMenuItem!=nullptr)
    {
      menu.addAction(GlobalGUI::freedesktopActive(),"Set Active",this,&InitiativeWidget::setActiveInitiativeEntry);
      QMenu* colorMenu = menu.addMenu("Set Color");
      colorMenu->setMinimumWidth(25);
      fillColorMenu(colorMenu);
      menu.addSeparator();
      menu.addAction(GlobalGUI::freedesktopTextEditor(),"Edit",this,&InitiativeWidget::editInitiativeEntry);
      menu.addAction(GlobalGUI::freedesktopCopy(),"Clone",this,&InitiativeWidget::clone);
      menu.addAction(GlobalGUI::freedesktopRemove(),"Remove",this,&InitiativeWidget::removeInitiativeEntry);
      menu.addSeparator();
    }
    menu.addAction(GlobalGUI::freedesktopAdd(),"New Entry",this,&InitiativeWidget::newInitiativeEntry);
    menu.addSeparator();
    menu.addAction(GlobalGUI::freedesktopSortAscending(),"Sort By Initiative",this,&InitiativeWidget::sortByInitiative);
    QMenu* iconSize = menu.addMenu(GlobalGUI::freedesktopZoomFitBest(),"Icon Size");
    iconSize->addAction("Small",this,&InitiativeWidget::iconSizeSmall);
    iconSize->addAction("Medium",this,&InitiativeWidget::iconSizeMedium);
    iconSize->addAction("Large",this,&InitiativeWidget::iconSizeLarge);
    iconSize->addAction("Huge",this,&InitiativeWidget::iconSizeHuge);
    iconSize->addAction("Gargantuan",this,&InitiativeWidget::iconSizeGargantuan);
    menu.exec(initiativeList->viewport()->mapToGlobal(pos));
  }
}

void InitiativeWidget::newInitiativeEntry()
{
  initiativeDialog->clear();
  if(initiativeDialog->exec())
  {
    InitiativeResource resource = initiativeDialog->getresource();
    orpg::ClientCore::get()->newInitiativeResource(resource);
  }
}

void InitiativeWidget::editInitiativeEntry()
{
  if(itemIdMap.contains(initiativeContextMenuItem))
  {
    int id = itemIdMap[initiativeContextMenuItem];
    Resource* resPtr = orpg::ClientCore::get()->getResources()->getInitiativeContainer()->getResourceById(id);
    if(resPtr==nullptr)
      return;
    InitiativeResource resource = *((InitiativeResource*)resPtr);
    initiativeDialog->setresource(resource);
    if(initiativeDialog->exec())
    {
      InitiativeResource editedresource = initiativeDialog->getresource();
      editedresource.setID(id);
      editedresource.setPosition(resource.getPosition());
      editedresource.setIsActive(resource.getIsActive());
      orpg::ClientCore::get()->setInitiativeResource(editedresource);
    }
  }
}

void InitiativeWidget::removeInitiativeEntry()
{
  if(initiativeContextMenuItem!=nullptr)
  {
    int id = itemIdMap[initiativeContextMenuItem];
    orpg::ClientCore::get()->removeInitiativeResource(id);
  }
}

void InitiativeWidget::setActiveInitiativeEntry()
{
  if(initiativeContextMenuItem!=nullptr)
  {
    int id = itemIdMap[initiativeContextMenuItem];
    orpg::ClientCore::get()->activateInitiativeResource(id);
  }
}

void InitiativeWidget::sortByInitiative()
{
  orpg::ClientCore::get()->sortInitiativeResources();
}

void InitiativeWidget::nextInitiative()
{
  orpg::ClientCore::get()->activateNextInitiativeResource();
}

void InitiativeWidget::itemDoubleClicked(QTreeWidgetItem *item, int)
{
  if(orpg::ClientCore::get()->getRole()<ROLE_GM)
    return;
  initiativeContextMenuItem = item;
  editInitiativeEntry();
}

void InitiativeWidget::roleChanged(PlayerRole role)
{
  if(role<ROLE_GM)
  {
    nextButton->setDisabled(true);
    initiativeList->setAcceptDrops(false);
    initiativeList->setDragEnabled(false);
  }
  else
  {
    nextButton->setDisabled(false);
    initiativeList->setAcceptDrops(true);
    initiativeList->setDragEnabled(true);
  }
}

void InitiativeWidget::imageComplete(const QUrl &url)
{
  updateImage(url,false);
}

void InitiativeWidget::imageFailed(const QUrl &url)
{
  updateImage(url,true);
}

void InitiativeWidget::moveItem(int fromPosition, int toPosition)
{
  int idFrom = itemIdMap[initiativeList->topLevelItem(fromPosition)];
  if(toPosition==initiativeList->topLevelItemCount())
  {
    QList<InitiativeResource> resources = orpg::ClientCore::get()->getResources()->getInitiativeContainer()->getResources();
    if(resources.size()>0)
      toPosition = resources.last().getPosition()+1;
    else
      toPosition = 0;
  }
  else
  {
    int toId = itemIdMap[initiativeList->topLevelItem(toPosition)];
    InitiativeResource* resource = (InitiativeResource*) orpg::ClientCore::get()->getResources()->getInitiativeContainer()->getResourceById(toId);
    toPosition = resource->getPosition();
  }
  orpg::ClientCore::get()->moveInitiativeResource(idFrom,toPosition);
}

void InitiativeWidget::setColor()
{
  if(initiativeContextMenuItem!=nullptr)
  {
    int id = itemIdMap[initiativeContextMenuItem];
    QColor color = ((QAction*)QObject::sender())->data().value<QColor>();
    orpg::ClientCore::get()->setInitiativeResourceColor(id,color.name());
  }
}

void InitiativeWidget::iconSizeSmall()
{
  setIconSize(16);
}

void InitiativeWidget::iconSizeMedium()
{
  setIconSize(32);
}

void InitiativeWidget::iconSizeLarge()
{
  setIconSize(64);
}

void InitiativeWidget::iconSizeHuge()
{
  setIconSize(128);
}

void InitiativeWidget::iconSizeGargantuan()
{
  setIconSize(256);
}
