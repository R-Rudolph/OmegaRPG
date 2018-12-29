#ifndef FAVOURITESSOURCE_H
#define FAVOURITESSOURCE_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSaveFile>
#include <QFile>
#include <QMenu>
#include <QMessageBox>
#include "serversource.h"
#include "gui/util/globalgui.h"
#include "servereditdialog.h"

class FavouritesSource : public ServerSource
{
  Q_OBJECT
  QTreeWidgetItem* rootItem;
  QTreeWidgetItem* currentItem;
  ServerList list;
  bool load(const QString& filename="");
  bool parse(const QJsonObject& favObject);
  QJsonObject toJson();
  void buildTree();
  void pushFavourite(const QString& address, const QString& name="");
public:
  explicit FavouritesSource(QObject* parent=0);
  virtual void execContextMenu(QTreeWidgetItem* item, const QPoint& pos);
  virtual QTreeWidgetItem* getTreeWidgetItem();
  void save(const QString& filename="");
public slots:
  void newFavourite(const QString& name, const QString& address);
private slots:
  void newEntryPressed();
  void editPressed();
  void removePressed();
};


#endif // FAVOURITESSOURCE_H
