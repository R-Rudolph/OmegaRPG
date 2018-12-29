#ifndef INITIATIVEWIDGET_H
#define INITIATIVEWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QPoint>

#include "core/multi/util/player.h"

class QMenu;
class InitiativeResource;
class InitiativeDialog;
class InitiativeTreeWidget;
class QPushButton;

class InitiativeWidget : public QWidget
{
  Q_OBJECT
  InitiativeDialog* initiativeDialog;
  QVBoxLayout* layout;
  InitiativeTreeWidget* initiativeList;
  QPushButton* nextButton;
  QTreeWidgetItem* initiativeContextMenuItem;

  QMap<QTreeWidgetItem*,int> itemIdMap;
  QTreeWidgetItem* activeInitiative;
  int lastPosition;

  void addItemFromResource(const InitiativeResource& resource);
  void updateImage(const QUrl& url, bool failed);
  void setIconSize(int value);
  QIcon fromColor(const QColor& color);
  QAction* colorAction(const QString& name, const QColor& color);
  void fillColorMenu(QMenu* menu);
public:
  explicit InitiativeWidget(QWidget *parent = nullptr);

signals:

private slots:
  void clone();
  void setInitiative(const QList<InitiativeResource>& initiative);
  void initiativeListContextMenu(const QPoint &pos);
  void newInitiativeEntry();
  void editInitiativeEntry();
  void removeInitiativeEntry();
  void setActiveInitiativeEntry();
  void sortByInitiative();
  void nextInitiative();
  void itemDoubleClicked(QTreeWidgetItem *item, int column);
  void roleChanged(PlayerRole role);
  void imageComplete(const QUrl& url);
  void imageFailed(const QUrl& url);
  void moveItem(int fromPosition, int toPosition);
  void setColor();
  void iconSizeSmall();
  void iconSizeMedium();
  void iconSizeLarge();
  void iconSizeHuge();
  void iconSizeGargantuan();
};

#endif // INITIATIVEWIDGET_H
