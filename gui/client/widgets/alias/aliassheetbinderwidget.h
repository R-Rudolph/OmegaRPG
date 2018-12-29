#ifndef ALIASSHEETBINDERWIDGET_H
#define ALIASSHEETBINDERWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QTabBar>
#include <QMenu>
#include <QInputDialog>
#include <QApplication>
#include <QClipboard>
#include <QLineEdit>
#include <QMessageBox>
#include "aliassheetwidget.h"
#include "core/client/alias/aliassheet.h"

class AliasSheetBinder;

class AliasSheetBinderWidget : public AliasSheetWidget
{
  Q_OBJECT
  QTabWidget* tabs;
  QList<AliasSheet*> getSheets();
  AliasSheetWidget* currentContextMenuSheet;
  bool nightMode;
public:
  explicit AliasSheetBinderWidget(bool edit, const QString& sheetName, QWidget *parent = 0);
  explicit AliasSheetBinderWidget(const AliasSheetBinder* sheet, bool edit, QWidget *parent = 0);
  virtual AliasSheet* toAliasSheet();
  void addSheetWidget(AliasSheetWidget* sheet);
signals:
public slots:
  virtual void setNightMode(bool value);
private slots:
  void import();
  void contextMenu(const QPoint& pos);
  void newBinder();
  void newStaticSheet();
  void newNotesSheet();
  void newDiceSheet();
  void contextRemoveSheet();
  void contextRenameSheet();
  void copyToClipboard();
  void pasteFromClipboard();
  void renameWidgetIndex(int index);
  void newPlugin();

private:
  void getPluginSubmenu(QMenu* menu);
};

#endif // ALIASSHEETBINDERWIDGET_H
