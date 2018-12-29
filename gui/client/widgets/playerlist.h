#ifndef PLAYERLIST_H
#define PLAYERLIST_H

#include <QVector>
#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QInputDialog>
#include "core/multi/util/player.h"

class PlayerList : public QWidget
{
  Q_OBJECT
  QVBoxLayout* layout;
  QTreeWidget* list;
  PlayerRole status;

  quint32 id;

  quint32 selectedPlayer;
  QString selectedPlayerName;

  QString gmpass;
  void assignStatusFunc(PlayerRole status);
public:
  explicit PlayerList(QWidget *parent = 0);
signals:
  void bootFromRoom(quint32 id);
  void assignStatus(quint32 id, PlayerRole status, QString gmpass);
  void openWhisper(quint32 id);
public slots:
  void setID(quint32 id);
  void setClientStatus(PlayerRole status);
  void setPlayers(QVector<Player> players);
private slots:
  void contextMenu(const QPoint & pos);
  void whisperSlotDoubleClick(QTreeWidgetItem* item,int);
  void whisperSlot();
  void bootSlot();
  void lurkerSlot();
  void playerSlot();
  void gmSlot();
};

#endif // PLAYERLIST_H
