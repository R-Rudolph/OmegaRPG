#ifndef HISTORYSOURCE_H
#define HISTORYSOURCE_H

#include "serversource.h"
#include "gui/util/globalgui.h"
#include "core/client/settings.h"
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSaveFile>

class HistorySource : public ServerSource
{
  Q_OBJECT
  QTreeWidgetItem* rootItem;
  QTreeWidgetItem* currentItem;
  void parse(const QJsonObject& histObj);
  void load(const QString& filename="");
  void save(const QString& filename="");
  QJsonObject toJson();
  void addEntryInternal(const QString& name, const QString& address);
public:
  explicit HistorySource(QObject* parent=0);
  virtual void execContextMenu(QTreeWidgetItem* item, const QPoint &pos);
  virtual QTreeWidgetItem* getTreeWidgetItem();
signals:
  void newFavourite(QString name, QString address);
public slots:
  void addEntry(const QString& name, const QString& address);
private slots:
  void clearHistoryPressed();
  void removeItemPressed();
  void addToFavouritesPressed();
};

#endif // HISTORYSOURCE_H
