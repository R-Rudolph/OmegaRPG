#include "aliassheetdicewidget.h"
#include "core/client/alias/aliassheetdice.h"
#include <QTextStream>
#include<QRandomGenerator>
#include "core/client/util/dice.h"

void AliasSheetDiceWidget::clearList()
{
  diceMap.clear();
  while(diceList->topLevelItemCount()>0)
    delete diceList->topLevelItem(0);
}

void AliasSheetDiceWidget::handleExpansion(QTreeWidgetItem *item)
{
  if(diceMap.contains(item))
  {
    item->setExpanded(diceMap[item]->getExpanded());
    for(int i=0;i<item->childCount();i++)
    {
      handleExpansion(item->child(i));
    }
  }
}

void AliasSheetDiceWidget::handleExpansion()
{
  for(int i=0;i<diceList->topLevelItemCount();i++)
    handleExpansion(diceList->topLevelItem(i));
}

QTreeWidgetItem *AliasSheetDiceWidget::itemFromDice(AliasDiceRoll &dice)
{
  return itemFromDice((AliasDiceRoll*)&dice);
}

QTreeWidgetItem *AliasSheetDiceWidget::itemFromDice(AliasDiceRoll *dice)
{
  QTreeWidgetItem* item = new QTreeWidgetItem(QStringList({dice->getName(),dice->getBonusString()}));
  item->setFlags(item->flags() | Qt::ItemIsDropEnabled);
  diceMap.insert(item,dice);
  for(int i=0;i<dice->getSubrolls().size();i++)
  {
    QTreeWidgetItem* subitem = itemFromDice(dice->getSubrolls()[i]);
    item->addChild(subitem);
  }
  if(item->childCount()!=0)
  {
    QFont font;
    font.setBold(true);
    item->setFont(0,font);
  }
  return item;
}

AliasDiceRoll AliasSheetDiceWidget::diceFromItem(QTreeWidgetItem *item)
{
  AliasDiceRoll newDice;
  newDice.setExpanded(item->isExpanded());
  newDice.setName(diceMap[item]->getName());
  newDice.setBonusString(diceMap[item]->getBonusString());
  newDice.setRollParent(diceMap[item]->getRollParent());
  for(int i=0;i<item->childCount();i++)
  {
    newDice.addSubroll(diceFromItem(item->child(i)));
  }
  return newDice;
}

int AliasSheetDiceWidget::rollDice(QPair<int, int> dice, QString &text, bool suppressPlus)
{
  text = QString();
  if(dice.second==0)
  {
    return 0;
  }
  int sum = 0;
  if(dice.first == 0)
  {
    sum = qAbs(dice.second);
    text = QString::number(qAbs(dice.second));
  }
  else
  {
    QTextStream t(&text);
    t << dice.first << "d" << qAbs(dice.second) << ":";
    for(int i=0;i<dice.first;i++)
    {
      int value = Dice::roll(dice.second);
      sum += value;
      t << value;
      if(i<dice.first-1)
        t << ",";
    }
  }
  if(dice.second>0)
  {
    if(suppressPlus)
      text = " "+text;
    else
      text = " + "+text;
  }
  else if(dice.second<0)
  {
    sum = -sum;
    text = " - "+text;
  }
  return sum;
}

void AliasSheetDiceWidget::buildListFromModel()
{
  clearList();
  for(int i=0;i<dice.getSubrolls().size();i++)
  {
    diceList->addTopLevelItem(itemFromDice(dice.getSubrolls()[i]));
  }
  handleExpansion();
}

void AliasSheetDiceWidget::buildModelFromList()
{
  AliasDiceRoll newDice;
  for(int i=0;i<diceList->topLevelItemCount();i++)
  {
    newDice.addSubroll(diceFromItem(diceList->topLevelItem(i)));
  }
  dice = newDice;
  buildListFromModel();
}

AliasSheetDiceWidget::AliasSheetDiceWidget(bool edit, const QString &sheetName, QWidget *parent) : AliasSheetWidget(edit,sheetName,parent)
{
  if(isEditWidget)
  {
    new QStackedLayout(this);
    diceList = new TreeWidgetExtended(this);
    diceList->setExpandsOnDoubleClick(false);
    diceList->setHeaderLabels(QStringList({"Name","Roll"}));
    diceList->setContextMenuPolicy(Qt::CustomContextMenu);
    diceList->setAcceptDrops(true);
    diceList->setDragEnabled(true);
    diceList->setDragDropMode(QAbstractItemView::InternalMove);
    diceList->header()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    connect(diceList,&QTreeWidget::customContextMenuRequested,this,&AliasSheetDiceWidget::contextMenu);
    connect(diceList,&QTreeWidget::itemDoubleClicked,this,&AliasSheetDiceWidget::editItem);
    connect(diceList,&TreeWidgetExtended::itemDropped,this,&AliasSheetDiceWidget::itemDropped);
    connect(diceList,&TreeWidgetExtended::itemExpanded,this,&AliasSheetDiceWidget::itemExpanded);
    connect(diceList,&TreeWidgetExtended::itemCollapsed,this,&AliasSheetDiceWidget::itemExpanded);
    layout()->addWidget(diceList);
  }
  else
  {
    new QVBoxLayout(this);
    diceList = new TreeWidgetExtended(this);
    diceList->setExpandsOnDoubleClick(false);
    diceList->setHeaderLabels(QStringList({"Name","Roll"}));
    diceList->setContextMenuPolicy(Qt::CustomContextMenu);
    diceList->header()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    modifierBox = new QSpinBox(this);
    modifierBox->setMinimum(-10000);
    modifierBox->setMaximum(10000);
    modifierBox->setValue(0);
    modifierBox->setToolTip("Modifier Box: This value is added to each roll.");
    connect(diceList,&QTreeWidget::itemDoubleClicked,this,&AliasSheetDiceWidget::itemRoll);
    layout()->addWidget(diceList);
    layout()->addWidget(modifierBox);
    connect(diceList,&QTreeWidget::customContextMenuRequested,this,&AliasSheetDiceWidget::contextMenuReadWidget);
  }
}

AliasSheetDiceWidget::AliasSheetDiceWidget(const AliasSheetDice *sheet, bool edit, QWidget *parent)
  : AliasSheetDiceWidget(edit,sheet->getSheetName(),parent)
{
  setDice(sheet->getRolls());
}

AliasSheet *AliasSheetDiceWidget::toAliasSheet()
{
  AliasSheetDice* sheet = new AliasSheetDice();
  sheet->setSheetName(sheetName);
  sheet->setRolls(dice);
  return sheet;
}

void AliasSheetDiceWidget::setDice(const AliasDiceRoll &dice)
{
  this->dice = dice;
  buildListFromModel();
}

void AliasSheetDiceWidget::contextMenu(const QPoint& pos)
{
  contextMenuCurrentItem = diceList->itemAt(pos);
  QMenu menu;
  menu.addAction("New",this,&AliasSheetDiceWidget::contextNewItem);
  menu.addSeparator();
  QAction* copyAction = menu.addAction("Copy",this,&AliasSheetDiceWidget::contextCopyItem);
  QAction* pasteAction = menu.addAction("Paste",this,&AliasSheetDiceWidget::contextPasteItem);
  menu.addSeparator();
  QAction* editAction = menu.addAction("Edit",this,&AliasSheetDiceWidget::contextEditItem);
  QAction* removeAction = menu.addAction("Remove",this,&AliasSheetDiceWidget::contextRemoveItem);
  if(contextMenuCurrentItem==nullptr)
  {
    copyAction->setDisabled(true);
    editAction->setDisabled(true);
    removeAction->setDisabled(true);
  }
  {
    QString clip = QApplication::clipboard()->text();
    QJsonDocument doc = QJsonDocument::fromJson(clip.toUtf8());
    AliasDiceRoll roll(doc.object());
    if(roll.isEmpty())
      pasteAction->setDisabled(true);
  }
  menu.exec(diceList->viewport()->mapToGlobal(pos));
}

void AliasSheetDiceWidget::contextMenuReadWidget(const QPoint &pos)
{
  contextMenuCurrentItem = diceList->itemAt(pos);
  if(contextMenuCurrentItem!=nullptr)
  {
    QMenu menu;
    menu.addAction("Roll",this,&AliasSheetDiceWidget::contextMenuRoll);
    menu.addSeparator();
    menu.addAction("Notes",this,&AliasSheetDiceWidget::contextMenuInfo);
    menu.exec(diceList->viewport()->mapToGlobal(pos));
  }
}

void AliasSheetDiceWidget::editItem(QTreeWidgetItem *item)
{
  if(diceMap.contains(item))
  {
    DiceRollEditDialog dialog(this);
    dialog.setName(diceMap[item]->getName());
    dialog.setBonus(diceMap[item]->getBonusString());
    dialog.setNotes(diceMap[item]->getNotes());
    dialog.setRollParent(diceMap[item]->getRollParent());
    if(dialog.exec())
    {
      if(diceMap.contains(item))
      {
        diceMap[item]->setName(dialog.getName());
        diceMap[item]->setBonusString(dialog.getBonus());
        diceMap[item]->setNotes(dialog.getNotes());
        diceMap[item]->setRollParent(dialog.getRollParent());
        buildListFromModel();
        emit changed();
      }
    }
  }
}

void AliasSheetDiceWidget::itemDropped()
{
  buildModelFromList();
  emit changed();
}

void AliasSheetDiceWidget::itemRoll(QTreeWidgetItem *item)
{
  QList<QPair<int,int> > completeRoll;
  QString name = diceMap[item]->getName();
  while(diceMap.contains(item))
  {
    completeRoll = diceMap[item]->getRolls() + completeRoll;
    if(!diceMap[item]->getRollParent())
      break;
    item = item->parent();
  }
  if(completeRoll.isEmpty())
    return;
  completeRoll.append(QPair<int,int>(0,modifierBox->value()));
  QString message;
  QTextStream t(&message);
  if(!name.isEmpty())
    t << name << ":";
  int sum = 0;
  for(int i=0;i<completeRoll.size();i++)
  {
    QString part;
    sum += rollDice(completeRoll[i],part,i==0);
    t << part;
  }
  t << " = " << sum;
  emit chat(message);
}

void AliasSheetDiceWidget::itemExpanded(QTreeWidgetItem *item)
{
  if(diceMap.contains(item))
  {
    diceMap[item]->setExpanded(item->isExpanded());
  }
}

void AliasSheetDiceWidget::contextNewItem()
{
  if(contextMenuCurrentItem==nullptr)
  {
    dice.addSubroll(AliasDiceRoll("New Roll",""));
    buildListFromModel();
    emit changed();
  }
  else
  {
    if(diceMap.contains(contextMenuCurrentItem))
    {
      diceMap[contextMenuCurrentItem]->addSubroll(AliasDiceRoll("New Roll",""));
      diceMap[contextMenuCurrentItem]->setExpanded(true);
    }
    buildListFromModel();
    emit changed();
  }
}

void AliasSheetDiceWidget::contextCopyItem()
{
  if(diceMap.contains(contextMenuCurrentItem))
  {
    QJsonDocument doc(diceMap[contextMenuCurrentItem]->toJson());
    QApplication::clipboard()->setText(QString::fromUtf8(doc.toJson()));
  }
}

void AliasSheetDiceWidget::contextPasteItem()
{
  QString clip = QApplication::clipboard()->text();
  QJsonDocument doc = QJsonDocument::fromJson(clip.toUtf8());
  AliasDiceRoll roll(doc.object());
  if(!roll.isEmpty())
  {
    if(contextMenuCurrentItem==nullptr)
    {
      dice.addSubroll(roll);
      buildListFromModel();
      emit changed();
    }
    else
    {
      if(diceMap.contains(contextMenuCurrentItem))
      {
        diceMap[contextMenuCurrentItem]->addSubroll(roll);
        diceMap[contextMenuCurrentItem]->setExpanded(true);
      }
      buildListFromModel();
      emit changed();
    }
  }
}

void AliasSheetDiceWidget::contextEditItem()
{
  editItem(contextMenuCurrentItem);
}

void AliasSheetDiceWidget::contextRemoveItem()
{
  delete contextMenuCurrentItem;
  buildModelFromList();
  emit changed();
}

void AliasSheetDiceWidget::contextMenuRoll()
{
  if(diceMap.contains(contextMenuCurrentItem))
  {
    itemRoll(contextMenuCurrentItem);
  }
}

void AliasSheetDiceWidget::contextMenuInfo()
{
  if(diceMap.contains(contextMenuCurrentItem))
  {
    DiceRollNotesDialog dialog(this,diceMap[contextMenuCurrentItem]->getNotes());
    dialog.exec();
  }
}
