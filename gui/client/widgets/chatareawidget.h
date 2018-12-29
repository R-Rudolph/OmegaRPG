#ifndef CHATAREAWIDGET_H
#define CHATAREAWIDGET_H

#include <QMap>
#include <QWidget>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QComboBox>
#include <QTimer>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QSplitter>
#include <QTabWidget>
#include <QAbstractItemView>
#include <QLabel>
#include <QIcon>
#include <QToolBar>
#include "chattextedit.h"
#include "core/client/alias/aliaslib.h"
#include "core/multi/util/player.h"
#include "widgets/colorselectbutton.h"
#include "core/client/settings.h"
#include "core/multi/message/chatmessage.h"


class ChatAreaWidget : public QWidget
{
  Q_OBJECT
  bool attentionStatus;
  QTimer* attentionTimer;
  QSet<QWidget*> attentionSet;
  QIcon attentionIcon;

  QTabWidget* chatTabs;
  ChatTextEdit* chatEdit;
  QPushButton* sheetSwitchButton;
  bool sheetSwitchButtonStatus;
  QComboBox* aliasBox;
  QSpinBox* numDiceBox;
  QSpinBox* bonusBox;
  QPushButton* d2;
  QPushButton* d3;
  QPushButton* d4;
  QPushButton* d6;
  QPushButton* d8;
  QPushButton* d10;
  QPushButton* d12;
  QPushButton* d20;
  QPushButton* d100;

  QPushButton* boldButton;
  QPushButton* italicButton;
  QPushButton* underlineButton;
  ColorSelectButton* colorButton;

  //layout stuff
  QSplitter* splitter;
  QHBoxLayout* topLayout;
  QVBoxLayout* mainLayout;
  QHBoxLayout* diceBarLayout;

  //other stuff
  QMap<QObject*,int> diceMap;
  QMap<QString, QTextBrowser*> chatBrowserMap;
  QMap<QTextBrowser*, QString> chatBrowserMapReverse;
  QMap<QWidget*, int> chatBrowserAliasMap;

  QFont font;
  QString dmTarget;
  QString myID;
  QMap<QString, QString> idMap;

  QString getTo();
  void addChatTab(const QString& title, const QString& id);
  void openTab(QString& id);
  QTextBrowser* getChatBrowser(const QString& from, const QString& to="");
  QString getNameByID(const QString& id);

  void initButton(QPushButton* button, int value);
public:
  explicit ChatAreaWidget(QWidget *parent = 0);

  QSplitter* getSplitter();
signals:
  void inactiveChannelMessage();
  void setSheetsVisible(bool);
  void typed();
  void idle();
  void chat(QString text, QString to);
  void notify();
  void kick(quint32 id);
public slots:
  void setPlayers(QVector<Player>);
  void updateAliasList();
  void openWhisperTab(quint32 id);
  void setID(quint32 id);
  void setFont(const QFont& font);
  void chatPrivate(QString text);
  void printErrorMessage(QString text);
  void printStatusMessage(QString text);
  void printChatMessage(const orpg::ChatMessage& chatMessage);
private slots:
  void sheetButtonPressed();
  void selectAlias(int);
  void updateAlias();
  void macroKey(int index);
  void typedPrivate();
  void diceButtonPressed(bool);
  void loadTabAlias(int);

  void tabSelected(int index);
  void closeTab(int index);
  void charFormatChanged(const QTextCharFormat& format);

  void attentionToggle();
};

#endif // CHATAREAWIDGET_H
