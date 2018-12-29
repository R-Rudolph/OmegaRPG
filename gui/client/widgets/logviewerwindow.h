#ifndef LOGVIEWERWINDOW_H
#define LOGVIEWERWINDOW_H

#include <QWidget>
#include <QList>
#include <QDir>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QTreeWidget>
#include <QTextBrowser>
#include <QMap>
#include <QMenu>
#include <QMenuBar>
#include <QCloseEvent>
#include <QPoint>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QThread>

#include "core/client/settings.h"
#include "core/client/log/sessionlogparser.h"
#include "widgets/searchabletextbrowser.h"
#include "widgets/coloroverlaywrapperwidget.h"

#include "core/client/log/logfilter.h"
#include "widgets/nightmodeoverlay.h"
#include "core/client/log/jsonlogparser.h"
#include "gui/client/objects/logprovider.h"

class LogViewerWidget;

class LogViewerWindow : public QMainWindow
{
  Q_OBJECT
  QSize lastSize;
  QPoint lastPosition;
  //
  LogProvider* logProvider;
  QThread* logProviderThread;
  //members
  QMap< QTreeWidgetItem*,QSharedPointer<orpg::JsonLogParser> > itemLogMap;
  QList<QTreeWidgetItem*> selectedItems;
  QList<orpg::LogFilter> filters;
  //widget stuff
  QVBoxLayout* verticalLayout;
  QSplitter* contentSplitter;
  QTreeWidget* fileList;
  LogViewerWidget* logViewer;
  //menu bar
  QMenu* fileMenu;
  QMenu* editMenu;
  QMenu* filterMenu;
  QMenu* removeFilterMenu;
  QAction* removeFilterSeperator;
  //member functions
  QTreeWidgetItem* getTreeWidgetItem(const QSharedPointer<orpg::JsonLogParser>& log);
public:
  explicit LogViewerWindow(QWidget *parent = 0);
  virtual ~LogViewerWindow();
  bool restoreGeometry(const QByteArray &geometry);
  QSplitter* getSplitter();
  void addFilter(const orpg::LogFilter& filter);
protected:
  void closeEvent(QCloseEvent* event);
signals:
  void filterAdded(orpg::LogFilter filter);
  void filterRemoved(orpg::LogFilter filter);
  void allFiltersRemoved();
  void refresh();
public slots:
  void show();
private slots:
  void deleteLogs();
  void manualFilter();
  void selectedFileChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
  void fileListContextMenu(const QPoint& pos);
  void addFilterClicked();
  void removeAllFilters();
  void removeFilter();

  void startedProvider();
  void finishedProvider();
  void addLog(QSharedPointer<orpg::JsonLogParser> log);
};

#endif // LOGVIEWERWINDOW_H
