#ifndef PLAYERLISTAREAWIDGET_H
#define PLAYERLISTAREAWIDGET_H

#include <QWidget>
#include <QSplitter>
#include <QStackedLayout>
#include <QDockWidget>
#include "widgets/playerlist.h"
#include "gui/client/widgets/alias/aliassheetreadwidget.h"

class PlayerListAreaWidget : public QWidget
{
  Q_OBJECT
  QStackedLayout* layout;
  QSplitter* splitter;
  PlayerList* playerList;
  AliasSheetReadWidget* aliasSheet;
public:
  explicit PlayerListAreaWidget(QWidget *parent = 0);
signals:
  void bootFromRoom(quint32 id);
  void assignStatus(quint32 id, PlayerRole status, QString dmpass);
  void openWhisper(quint32 id);
  void chat(const QString& text);
public slots:
  void setID(quint32 id);
  void setClientStatus(PlayerRole status);
  void setPlayers(QVector<Player> players);
  void setAliasSheetVisible(bool value);
};

#endif // PLAYERLISTAREAWIDGET_H
