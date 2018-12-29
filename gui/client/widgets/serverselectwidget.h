#ifndef SERVERSELECTWIDGET_H
#define SERVERSELECTWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QTreeWidget>
#include <QLineEdit>
#include <QMap>

#include "core/multi/util/serverlist.h"
#include "core/client/serversource/favouritessource.h"
#include "core/client/serversource/historysource.h"
#include "core/client/serversource/metaserversource.h"

class ServerSelectWidget : public QWidget
{
  Q_OBJECT
  QSet<ServerSource*> sources;
  MetaServerSource* meta;
  FavouritesSource* fav;
  HistorySource* hist;

  QGridLayout* layout;
  QLineEdit* addressLine;
  QPushButton* connectButton;
  QPushButton* _disconnectButton;
  QPushButton* refreshButton;
  QPushButton* serverButton_;
  QTreeWidget* serverTree;

public:
  explicit ServerSelectWidget(QWidget *parent = nullptr);

  QPushButton* disconnectButton() const;

signals:
  void connectToHost(QString address);
  void disconnectFromHost();
  void refresh();
  void serverMenu();
public slots:
  void save();
private slots:
  void selectServer(QTreeWidgetItem* item, QTreeWidgetItem* prevItem);
  void connectAddr();
  void contextMenu(QPoint point);
};

#endif // SERVERSELECTWIDGET_H
