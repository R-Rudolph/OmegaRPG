#ifndef ALIASSHEETDICEWIDGET_H
#define ALIASSHEETDICEWIDGET_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include <QMap>
#include <QMenu>
#include "widgets/treewidgetextended.h"
#include "widgets/treewidgetitemextended.h"
#include "aliassheetwidget.h"
#include "dialogs/dicerolleditdialog.h"
#include "dialogs/dicerollnotesdialog.h"

class AliasSheetDice;

class AliasSheetDiceWidget : public AliasSheetWidget
{
  Q_OBJECT
  TreeWidgetExtended* diceList;
  QSpinBox* modifierBox;

  AliasDiceRoll dice;
  QMap<QTreeWidgetItem*,AliasDiceRoll*> diceMap;

  QTreeWidgetItem* contextMenuCurrentItem;
  void clearList();
  void handleExpansion(QTreeWidgetItem* item);
  void handleExpansion();
  QTreeWidgetItem *itemFromDice(AliasDiceRoll &dice);
  QTreeWidgetItem *itemFromDice(AliasDiceRoll* dice);
  AliasDiceRoll diceFromItem(QTreeWidgetItem* item);
  int rollDice(QPair<int,int> dice, QString& text, bool suppressPlus=false);
  void buildListFromModel();
  void buildModelFromList();
public:
  explicit AliasSheetDiceWidget(bool edit, const QString& sheetName, QWidget *parent = nullptr);
  explicit AliasSheetDiceWidget(const AliasSheetDice* sheet, bool edit, QWidget *parent = nullptr);
  virtual AliasSheet* toAliasSheet();
  void setDice(const AliasDiceRoll& dice);
signals:

private slots:
  void contextMenu(const QPoint& pos);
  void contextMenuReadWidget(const QPoint& pos);
  void editItem(QTreeWidgetItem* item);
  void itemDropped();
  void itemRoll(QTreeWidgetItem* item);
  void itemExpanded(QTreeWidgetItem* item);
  void contextNewItem();
  void contextCopyItem();
  void contextPasteItem();
  void contextEditItem();
  void contextRemoveItem();
  void contextMenuRoll();
  void contextMenuInfo();
};

#endif // ALIASSHEETDICEWIDGET_H
