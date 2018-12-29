#include "aliaswindow.h"
#include "globalgui.h"

QSplitter *AliasWindow::getSplitter() const
{
  return splitter;
}

void AliasWindow::fillAliasTree()
{
  clearAliasTree();
  editingCheckedState = true;
  for(int i=0;i<localLib.sets.size();i++)
  {
    QTreeWidgetItem* item = makeItem(localLib.sets[i]);
    aliasTree->addTopLevelItem(item);
    item->setExpanded(localLib.sets[i].selected);
  }
  editingCheckedState = false;
  fixAliasTreeCheckmarks();
}

void AliasWindow::fixAliasTreeCheckmarks()
{
  editingCheckedState = true;
  for(int i=0;i<aliasTree->topLevelItemCount();i++)
  {
    Qt::CheckState state = fixAliasTreeCheckmarks(aliasTree->topLevelItem(i));
    aliasTree->topLevelItem(i)->setCheckState(0,state);
  }
  editingCheckedState = false;
  currentAliasChanged(0,0);
}

Qt::CheckState AliasWindow::fixAliasTreeCheckmarks(QTreeWidgetItem *item)
{
  if(itemSetMap.contains(item))
  {
    item->setExpanded(itemSetMap[item]->selected);
    bool allChecked = true;
    bool allUnchecked = true;
    for(int i=0;i<item->childCount();i++)
    {
      Qt::CheckState state = fixAliasTreeCheckmarks(item->child(i));
      item->child(i)->setCheckState(0,state);
      if(state!=Qt::Checked)
        allChecked = false;
      if(state!=Qt::Unchecked)
        allUnchecked = false;
    }
    if(allChecked && allUnchecked)
      return Qt::Unchecked; //unchecked, since empty
    else if(!allChecked && !allUnchecked)
      return Qt::PartiallyChecked;
    else if(allChecked && !allUnchecked)
      return Qt::Checked;
    else
      return Qt::Unchecked;
  }
  else if(itemAliasMap.contains(item))
  {
    if(itemAliasMap[item]->getSelected())
      return Qt::Checked;
    else
      return Qt::Unchecked;
  }
  return Qt::Unchecked;
}

void AliasWindow::clearAliasTree()
{
  itemSetMap.clear();
  itemAliasMap.clear();
  while(aliasTree->topLevelItemCount()>0)
    delete aliasTree->topLevelItem(0);
}

QTreeWidgetItem *AliasWindow::makeItem(AliasSet &set)
{
  QTreeWidgetItem* item = new TreeWidgetItemExtended();
  item->setText(0,set.name);
  item->setFlags(item->flags() | Qt::ItemIsDropEnabled | Qt::ItemIsUserCheckable);
  QFont f = item->font(0);
  f.setBold(true);
  item->setFont(0,f);
  item->setIcon(0,GlobalGUI::freedesktopFolder());
  //TODO: set properties
  itemSetMap.insert(item,&set);
  for(int i=0;i<set.subsets.size();i++)
  {
    QTreeWidgetItem* subitem = makeItem(set.subsets[i]);
    item->addChild(subitem);
  }
  for(int i=0;i<set.set.size();i++)
  {
    QTreeWidgetItem* subitem = makeItem(set.set[i]);
    item->addChild(subitem);
  }
  return item;
}

QTreeWidgetItem *AliasWindow::makeItem(Alias &alias)
{
  QTreeWidgetItem* item = new TreeWidgetItemExtended();
  item->setText(0,alias.getName());
  item->setFlags((item->flags() & (~Qt::ItemIsDropEnabled)) | Qt::ItemIsUserCheckable);
  //TODO: set properties
  itemAliasMap.insert(item,&alias);
  return item;
}

AliasSet AliasWindow::makeSet(QTreeWidgetItem *item)
{
  AliasSet set(itemSetMap[item]->name);
  set.selected = item->isExpanded();
  for(int i=0;i<item->childCount();i++)
  {
    QTreeWidgetItem* child = item->child(i);
    if(itemSetMap.contains(child))
    {
      set.subsets.append(makeSet(child));
    }
    else if(itemAliasMap.contains(child))
    {
      set.set.append(*itemAliasMap[child]);
    }
  }
  return set;
}

void AliasWindow::setCheckedState(QTreeWidgetItem *item, Qt::CheckState state)
{
  if(itemSetMap.contains(item))
  {
    for(int i=0;i<item->childCount();i++)
    {
      setCheckedState(item->child(i),state);
    }
  }
  else if(itemAliasMap.contains(item))
  {
    itemAliasMap[item]->setSelected(state==Qt::Checked);
  }
}

void AliasWindow::setTitle(bool unsavedChanges)
{
  this->unsavedChanges = unsavedChanges;
  if(!unsavedChanges)
    setWindowTitle("Alias Library");
  else
    setWindowTitle("Alias Library *");
}

void AliasWindow::restructure()
{
  QVector<AliasSet> sets;
  for(int i=0;i<aliasTree->topLevelItemCount();i++)
  {
    if(itemSetMap.contains(aliasTree->topLevelItem(i)))
    {
      sets.append(makeSet(aliasTree->topLevelItem(i)));
    }
  }
  localLib.sets = sets;
  fillAliasTree();
}

AliasWindow::AliasWindow(QWidget *parent) : NightModeOverlay(parent,Qt::Window)
{
  suppressChange = false;
  editingCheckedState = false;
  this->setWidget(new QWidget(this));
  mainLayout = new QStackedLayout(this->getWidget());
  splitter = new QSplitter(Qt::Horizontal,this);
  aliasTree = new TreeWidgetExtended(this);
  rightWidget = new QWidget(this);
  rightLayout = new QVBoxLayout(rightWidget);
  controlLayout = new QStackedLayout();
  buttonLayout = new QHBoxLayout();
  okButton = new QPushButton("Ok",this);
  cancelButton = new QPushButton("Cancel",this);
  noneControl = new QWidget(this);
  aliasControl = new AliasEditWidget(this);
  aliasSetControl = new AliasSetEditWidget(this);

  mainLayout->addWidget(splitter);
  splitter->addWidget(aliasTree);
  splitter->addWidget(rightWidget);
  rightLayout->addLayout(controlLayout);
  rightLayout->addLayout(buttonLayout);
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);
  controlLayout->addWidget(noneControl);
  controlLayout->addWidget(aliasControl);
  controlLayout->addWidget(aliasSetControl);

  rightLayout->setStretch(0,1);
  rightLayout->setStretch(1,0);
  aliasTree->setHeaderHidden(true);
  aliasTree->setAcceptDrops(true);
  aliasTree->setDragEnabled(true);
  aliasTree->setDragDropMode(QAbstractItemView::InternalMove);
  aliasTree->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(okButton,&QPushButton::clicked,this,&AliasWindow::ok);
  connect(cancelButton,&QPushButton::clicked,this,&AliasWindow::close);
  connect(aliasTree,&QTreeWidget::currentItemChanged,this,&AliasWindow::currentAliasChanged);
  connect(aliasTree,&QTreeWidget::customContextMenuRequested,this,&AliasWindow::aliasTreeContextMenu);
  connect(aliasTree,&QTreeWidget::itemExpanded,this,&AliasWindow::aliasTreeItemExpanded);
  connect(aliasTree,&QTreeWidget::itemCollapsed,this,&AliasWindow::aliasTreeItemExpanded);
  connect(aliasTree,&TreeWidgetExtended::itemCheckedStateChanged,this,&AliasWindow::checkedStateChanged);
  connect(aliasControl,&AliasEditWidget::nameChanged,this,&AliasWindow::nameChanged);
  connect(aliasSetControl,&AliasSetEditWidget::nameChanged,this,&AliasWindow::nameChanged);
  connect(AliasLib::get(),&AliasLib::libraryChanged,this,&AliasWindow::loadLib);
  connect(aliasTree,&TreeWidgetExtended::itemDropped,this,&AliasWindow::restructure);
  connect(aliasControl,&AliasEditWidget::changed,this,&AliasWindow::changeHappend);
  connect(aliasSetControl,&AliasSetEditWidget::changed,this,&AliasWindow::changeHappend);
  loadLib();
}

bool AliasWindow::restoreGeometry(const QByteArray &geometry)
{
  bool result = QWidget::restoreGeometry(geometry);
  return result;
}

void AliasWindow::closeEvent(QCloseEvent *event)
{
  if(unsavedChanges)
  {
    QMessageBox::StandardButton button = QMessageBox::question(this,
                                                               "Save Changes",
                                                               "The alias library has unsaved changes. Quit anyway?",
                                                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                                               QMessageBox::Discard);
    switch(button)
    {
      case QMessageBox::Save:
      {
        restructure();
        AliasLib::get()->set(localLib);
        setTitle(false);
        event->accept();
        break;
      }
      case QMessageBox::Discard:
      {
        lastSize = QWidget::size();
        lastPosition = QWidget::pos();
        localLib.set(AliasLib::get());
        fillAliasTree();
        setTitle(false);
        event->accept();
        break;
      }
      case QMessageBox::Cancel:
      default:
      {
        event->ignore();
        break;
      }
    }
  }
  else
  {
    lastSize = QWidget::size();
    lastPosition = QWidget::pos();
    localLib.set(AliasLib::get());
    fillAliasTree();
    setTitle(false);
    event->accept();
  }
}

void AliasWindow::show()
{
  QWidget::show();
  if(lastSize.isValid())
  {
    resize(QSize(0,0));
    move(lastPosition);
    resize(lastSize);
  }
}

void AliasWindow::changeHappend()
{
  if(!suppressChange)
    setTitle(true);
}

void AliasWindow::importOmegaRpg()
{
  QString filename = QFileDialog::getOpenFileName(this,"Import Alias Library",QString(),"JSON file (*.json);;All files (*)");
  if(!filename.isEmpty())
  {
    if(localLib.import(filename))
    {
      fillAliasTree();
      setTitle(true);
    }
    else
    {
      QMessageBox::warning(this,"Import Error","Error while importing:\n"+localLib.error(),QMessageBox::Ok);
    }
  }
}

void AliasWindow::importOpenRpg()
{
  QString filename = QFileDialog::getOpenFileName(this,"Import OpenRPG Alias Library",QString(),"OpenRPG alias file (*.alias);;All files (*)");
  if(!filename.isEmpty())
  {
    if(localLib.importOpenRPG(filename))
    {
      fillAliasTree();
      setTitle(true);
    }
    else
    {
      QMessageBox::warning(this,"Import Error","Error while importing:\n"+localLib.error(),QMessageBox::Ok);
    }
  }
}

void AliasWindow::currentAliasChanged(QTreeWidgetItem *current, QTreeWidgetItem *)
{
  suppressChange = true;
  if(itemAliasMap.contains(current))
  {
    aliasTree->invisibleRootItem()->setFlags(aliasTree->invisibleRootItem()->flags() & (~Qt::ItemIsDropEnabled));
    aliasControl->setAlias(itemAliasMap[current]);
    controlLayout->setCurrentWidget(aliasControl);
  }
  else if(itemSetMap.contains(current))
  {
    aliasTree->invisibleRootItem()->setFlags(aliasTree->invisibleRootItem()->flags() | (Qt::ItemIsDropEnabled));
    aliasSetControl->setAliasSet(itemSetMap[current]);
    controlLayout->setCurrentWidget(aliasSetControl);
  }
  else
    controlLayout->setCurrentWidget(noneControl);
  suppressChange = false;
}

void AliasWindow::ok()
{
  restructure();
  AliasLib::get()->set(localLib);
  setTitle(false);
  close();
}

void AliasWindow::loadLib()
{
  localLib.set(AliasLib::get());
  fillAliasTree();
  setTitle(false);
}

void AliasWindow::aliasTreeContextMenu(const QPoint &pos)
{
  contextMenuCurrentItem = aliasTree->itemAt(pos);
  QMenu menu(this);
  QAction* newAliasAction = menu.addAction(GlobalGUI::freedesktopAdd(),"New Alias",this,&AliasWindow::contextMenuNewAliasAction);
  if(contextMenuCurrentItem==nullptr)
  {
    newAliasAction->setToolTip("This is only possible within alias folders.");
    newAliasAction->setDisabled(true);
  }
  menu.addAction(GlobalGUI::freedesktopNewFolder(),"New Set",this,&AliasWindow::contextMenuNewSetAction);
  QMenu* importMenu = menu.addMenu(GlobalGUI::freedesktopLoad(),"Import");
  importMenu->addAction("OmegaRPG Library",this,&AliasWindow::importOmegaRpg);
  importMenu->addAction("OpenRPG Library",this,&AliasWindow::importOpenRpg);
  menu.addSeparator();
  QAction* copyAction = menu.addAction(GlobalGUI::freedesktopCopy(),"Copy",this,&AliasWindow::contextMenuCopyAction);
  QAction* removeAction = menu.addAction(GlobalGUI::freedesktopRemove(),"Remove",this,&AliasWindow::contextMenuRemoveAction);
  if(contextMenuCurrentItem==nullptr)
  {
    copyAction->setDisabled(true);
    removeAction->setDisabled(true);
  }
  menu.exec(aliasTree->viewport()->mapToGlobal(pos));
}

void AliasWindow::contextMenuCopyAction()
{
  if(itemAliasMap.contains(contextMenuCurrentItem))
  {
    Alias* alias = itemAliasMap[contextMenuCurrentItem];
    QTreeWidgetItem* setItem = contextMenuCurrentItem->parent();
    AliasSet* set = itemSetMap[setItem];
    set->set.append(*alias);
    fillAliasTree();
    setTitle(true);
  }
  else if(itemSetMap.contains(contextMenuCurrentItem))
  {
    AliasSet* set = itemSetMap[contextMenuCurrentItem];
    QTreeWidgetItem* parentItem = contextMenuCurrentItem->parent();
    if(parentItem!=nullptr)
    {
      AliasSet* pset = itemSetMap[parentItem];
      pset->subsets.append(*set);
    }
    else
    {
      localLib.sets.append(*set);
    }
    fillAliasTree();
    setTitle(true);
  }
}

void AliasWindow::contextMenuRemoveAction()
{
  int answer = QMessageBox::question(this,
                                     "Remove Alias Element",
                                     "Are you sure you want to delete this Alias Element?",
                                     QMessageBox::Yes | QMessageBox::No,
                                     QMessageBox::No);
  if(answer==QMessageBox::Yes)
  {
    delete contextMenuCurrentItem;
    restructure();
    setTitle(true);
  }
}

void AliasWindow::contextMenuNewSetAction()
{
  if(contextMenuCurrentItem==nullptr)
  {
    localLib.addSet("New Set");
    fillAliasTree();
    setTitle(true);
  }
  else if(itemAliasMap.contains(contextMenuCurrentItem))
  {
    if(itemSetMap.contains(contextMenuCurrentItem->parent()))
    {
      itemSetMap[contextMenuCurrentItem->parent()]->addSet("New Set");
      itemSetMap[contextMenuCurrentItem->parent()]->selected = true;
      fillAliasTree();
      setTitle(true);
    }
  }
  else if(itemSetMap.contains(contextMenuCurrentItem))
  {
    itemSetMap[contextMenuCurrentItem]->addSet("New Set");
    itemSetMap[contextMenuCurrentItem]->selected = true;
    fillAliasTree();
    setTitle(true);
  }
}

void AliasWindow::contextMenuNewAliasAction()
{
  if(itemAliasMap.contains(contextMenuCurrentItem))
    contextMenuCurrentItem = contextMenuCurrentItem->parent();
  if(itemSetMap.contains(contextMenuCurrentItem))
  {
    itemSetMap[contextMenuCurrentItem]->addAlias();
    itemSetMap[contextMenuCurrentItem]->selected = true;
    fillAliasTree();
    setTitle(true);
  }
}

void AliasWindow::aliasTreeItemExpanded(QTreeWidgetItem *item)
{
  if(itemSetMap.contains(item))
  {
    itemSetMap[item]->selected = item->isExpanded();
  }
}

void AliasWindow::checkedStateChanged(QTreeWidgetItem *item, Qt::CheckState state)
{
  if(!editingCheckedState && (itemSetMap.contains(item) || itemAliasMap.contains(item)))
  {
    setCheckedState(item,state);
    fixAliasTreeCheckmarks();
    setTitle(true);
  }
}

void AliasWindow::nameChanged(const QString &name)
{
  if(aliasTree->currentItem()!=nullptr)
  {
    if(aliasTree->currentItem()->text(0) != name)
    {
      aliasTree->currentItem()->setText(0,name);
      setTitle(true);
    }
  }
}
