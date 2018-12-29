#ifndef SERVERSOURCE_H
#define SERVERSOURCE_H

#include <QObject>
#include <QMenu>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include "core/multi/util/serverlist.h"
#include "core/multi/util/serverentry.h"

class ServerSource : public QObject
{
  Q_OBJECT
public:
  explicit ServerSource(QObject *parent = 0);
  virtual void execContextMenu(QTreeWidgetItem* item, const QPoint &pos);
  virtual QTreeWidgetItem* getTreeWidgetItem();
signals:

public slots:
};

#endif // SERVERSOURCE_H
