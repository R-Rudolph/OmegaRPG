#ifndef ALIASWINDOW_H
#define ALIASWINDOW_H

#include <QWidget>
#include <QSplitter>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QPushButton>
#include <QStackedLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QFileDialog>
#include <QCloseEvent>
#include <QMap>
#include <QMenu>

#include "core/client/alias/aliaslib.h"
#include "widgets/treewidgetextended.h"
#include "widgets/treewidgetitemextended.h"
#include "widgets/alias/aliaseditwidget.h"
#include "widgets/alias/aliasseteditwidget.h"
#include "widgets/nightmodeoverlay.h"

class AliasWindow : public NightModeOverlay
{
  Q_OBJECT
  QSize lastSize;
  QPoint lastPosition;
  AliasLib localLib;
  QStackedLayout* mainLayout;
  QSplitter* splitter;
  TreeWidgetExtended* aliasTree;
  QWidget* rightWidget;
  QVBoxLayout* rightLayout;
  QStackedLayout* controlLayout;
  QHBoxLayout* buttonLayout;
  QPushButton* okButton;
  QPushButton* cancelButton;
  QWidget* noneControl;
  AliasEditWidget* aliasControl;
  AliasSetEditWidget* aliasSetControl;
  bool unsavedChanges;

  bool suppressChange;
  bool editingCheckedState;
  QTreeWidgetItem* contextMenuCurrentItem;
  QMap<QTreeWidgetItem*,AliasSet*> itemSetMap;
  QMap<QTreeWidgetItem*,Alias*> itemAliasMap;

  void fillAliasTree();
  void fixAliasTreeCheckmarks();
  Qt::CheckState fixAliasTreeCheckmarks(QTreeWidgetItem* item);
  void clearAliasTree();
  QTreeWidgetItem* makeItem(AliasSet& set);
  QTreeWidgetItem* makeItem(Alias& alias);
  AliasSet makeSet(QTreeWidgetItem* item);
  void setCheckedState(QTreeWidgetItem* item, Qt::CheckState);
  void setTitle(bool unsavedChanges);
public:
  explicit AliasWindow(QWidget *parent = 0);
  bool restoreGeometry(const QByteArray &geometry);
  QSplitter *getSplitter() const;

protected:
  void closeEvent(QCloseEvent *event);
signals:
public slots:
  void show();
private slots:
  void changeHappend();
  void importOmegaRpg();
  void importOpenRpg();
  void restructure();
  void currentAliasChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
  void ok();
  void loadLib();
  void aliasTreeContextMenu(const QPoint& pos);
  void contextMenuCopyAction();
  void contextMenuRemoveAction();
  void contextMenuNewSetAction();
  void contextMenuNewAliasAction();
  void aliasTreeItemExpanded(QTreeWidgetItem* item);
  void checkedStateChanged(QTreeWidgetItem* item, Qt::CheckState state);
  void nameChanged(const QString& name);
};

#endif // ALIASWINDOW_H
