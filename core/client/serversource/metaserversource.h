#ifndef METASERVERSOURCE_H
#define METASERVERSOURCE_H

#include <QObject>
#include <QNetworkReply>
#include <QMap>
#include <QMessageBox>
#include "serversource.h"
#include "core/multi/metaserverclient.h"
#include "core/multi/util/global.h"
#include "core/client/settings.h"

class MetaServerSource : public ServerSource
{
  Q_OBJECT
  bool error;
  QString errorMsg;
  QBrush brush;

  QTreeWidgetItem* rootItem;
  QTreeWidgetItem* currentItem;
  MetaServerClient* metaServerClient;
  void clearAllChildren();
  QTreeWidgetItem* itemFromServer(const ServerEntry& server);
public:
  explicit MetaServerSource(QObject *parent = 0);
  virtual void execContextMenu(QTreeWidgetItem* item, const QPoint &pos);
  virtual QTreeWidgetItem* getTreeWidgetItem();
signals:
  void newFavourite(QString name, QString address);
public slots:
  void refresh();
  void status();
private slots:
  void setServerList(ServerList list);
  void getFailed(QNetworkReply::NetworkError error, QString errorMsg);
  void addToFavoritesPressed();
  void statusChanged(QMap<QString,MetaServerClient::MetaServerStatus>);
};

#endif // METASERVERSOURCE_H
