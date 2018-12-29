#include "logviewerwindow.h"
#include "objects/nightmode.h"
#include "dialogs/logfilterdialog.h"
#include "widgets/logviewerwidget.h"

#include<QHeaderView>

QTreeWidgetItem *LogViewerWindow::getTreeWidgetItem(const QSharedPointer<orpg::JsonLogParser>& log)
{
  QTreeWidgetItem* item = new QTreeWidgetItem(QStringList({log->getTime().toString("yyyy-MM-dd"),log->getRoomName(),log->getFilename(),log->getServerName()}));
  itemLogMap.insert(item,log);
  return item;
}

LogViewerWindow::LogViewerWindow(QWidget *parent) : QMainWindow(parent,Qt::Window)
{
  setWindowTitle("OmegaRPG Log Viewer");
  NightModeOverlay* nightmodeOverlay = new NightModeOverlay(this);
  nightmodeOverlay->setWidget(new QWidget(nightmodeOverlay));
  setCentralWidget(nightmodeOverlay);
  verticalLayout = new QVBoxLayout(nightmodeOverlay->getWidget());
  contentSplitter = new QSplitter(Qt::Horizontal,this);
  fileList = new QTreeWidget(this);
  logViewer = new LogViewerWidget(this);

  verticalLayout->addWidget(contentSplitter);
  verticalLayout->setMargin(2);

  contentSplitter->addWidget(fileList);
  contentSplitter->addWidget(logViewer);

  fileList->setHeaderLabels(QStringList({"Date","Name","File","Server"}));
  fileList->setRootIsDecorated(false);
  fileList->setContextMenuPolicy(Qt::CustomContextMenu);
  fileList->setSelectionMode(QAbstractItemView::ExtendedSelection);
  fileList->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

  //set up tab bar
  fileMenu = menuBar()->addMenu("File");
  fileMenu->addAction("Refresh",this,&LogViewerWindow::refresh);
  fileMenu->addSeparator();
  fileMenu->addAction("Close",this,&LogViewerWindow::close);
  editMenu = menuBar()->addMenu("Edit");
  editMenu->addAction("Find",logViewer,&LogViewerWidget::showSearchBar,QKeySequence("Ctrl+F"));
  filterMenu = menuBar()->addMenu("Filter");
  filterMenu->addAction("Add Filter",this,&LogViewerWindow::addFilterClicked);
  removeFilterMenu = filterMenu->addMenu("Remove Filter");
  removeFilterMenu->setDisabled(true);
  removeFilterSeperator = removeFilterMenu->addSeparator();
  removeFilterMenu->addAction("Remove All",this,&LogViewerWindow::removeAllFilters);

  //connections
  connect(fileList,&QTreeWidget::currentItemChanged,this,&LogViewerWindow::selectedFileChanged);
  connect(fileList,&QTreeWidget::customContextMenuRequested,this,&LogViewerWindow::fileListContextMenu);

  //log provider
  logProviderThread = new QThread(this);
  logProvider = new LogProvider();
  logProvider->moveToThread(logProviderThread);
  orpg::JsonLogParser::registerTypes();
  orpg::LogFilter::registerTypes();
  connect(logProvider,&LogProvider::started, this,&LogViewerWindow::startedProvider);
  connect(logProvider,&LogProvider::finished,this,&LogViewerWindow::finishedProvider);
  connect(logProvider,&LogProvider::foundLog,this,&LogViewerWindow::addLog);

  connect(this,&LogViewerWindow::filterAdded,      logProvider,&LogProvider::addFilter);
  connect(this,&LogViewerWindow::filterRemoved,    logProvider,&LogProvider::removeFilter);
  connect(this,&LogViewerWindow::allFiltersRemoved,logProvider,&LogProvider::removeAllFilters);
  connect(this,&LogViewerWindow::refresh,          logProvider,&LogProvider::refresh);
  logProviderThread->start();
}

LogViewerWindow::~LogViewerWindow()
{
  logProviderThread->quit();
  logProviderThread->wait();
}

bool LogViewerWindow::restoreGeometry(const QByteArray &geometry)
{
  bool result = QWidget::restoreGeometry(geometry);
  return result;
}

QSplitter *LogViewerWindow::getSplitter()
{
  return contentSplitter;
}

void LogViewerWindow::closeEvent(QCloseEvent *event)
{
  lastSize = QWidget::size();
  lastPosition = QWidget::pos();
  event->accept();
}

void LogViewerWindow::show()
{
  QWidget::show();
  if(lastSize.isValid())
  {
    resize(QSize(0,0));
    move(lastPosition);
    resize(lastSize);
  }
  emit refresh();
}

void LogViewerWindow::deleteLogs()
{
  QMessageBox::StandardButton pressedButton = QMessageBox::critical(this,
                                                                    "Log Deletion",
                                                                    "Are you sure you want to delete the selected "+QString::number(selectedItems.size())+" log(s) permanently?",
                                                                    QMessageBox::Yes|QMessageBox::No,
                                                                    QMessageBox::No);
  if(pressedButton == QMessageBox::Yes)
  {
    foreach(QTreeWidgetItem* item,selectedItems)
    {
      QFile::remove(itemLogMap[item]->getFilename());
      delete item;
    }
    selectedItems.clear();
  }
}

void LogViewerWindow::manualFilter()
{
  QList<QString> logUIDs;
  foreach(QTreeWidgetItem* item,selectedItems)
  {
    logUIDs.append(itemLogMap[item]->getUID());
  }
  addFilter(orpg::LogFilter(logUIDs));
}

void LogViewerWindow::selectedFileChanged(QTreeWidgetItem *current, QTreeWidgetItem*)
{
  if(itemLogMap.contains(current))
  {
    logViewer->loadFile(itemLogMap[current]->getFilename());
  }
}

void LogViewerWindow::fileListContextMenu(const QPoint &pos)
{
  selectedItems = fileList->selectedItems();
  if(selectedItems.size()>0)
  {
    QMenu menu(this);
    menu.addAction("Filter",this,&LogViewerWindow::manualFilter);
    menu.addAction("Delete Selection",this,&LogViewerWindow::deleteLogs);
    menu.exec(fileList->viewport()->mapToGlobal(pos));
  }
}

void LogViewerWindow::addFilter(const orpg::LogFilter& filter)
{
  filters.append(filter);
  QAction* newAction = new QAction(filter.description());
  connect(newAction,&QAction::triggered,this,&LogViewerWindow::removeFilter);
  removeFilterMenu->insertAction(removeFilterSeperator,newAction);
  removeFilterMenu->setDisabled(false);
  emit filterAdded(filter);
}

void LogViewerWindow::addFilterClicked()
{
  LogFilterDialog dialog;
  if(dialog.exec())
  {
    addFilter(dialog.getFilter());
  }
}

void LogViewerWindow::removeAllFilters()
{
  filters.clear();
  removeFilterMenu->setDisabled(true);
  QList<QAction*> actions = removeFilterMenu->actions();
  for(int i=0;i<actions.size()-2;i++)
  {
    actions[i]->deleteLater();
  }
  emit allFiltersRemoved();
}

void LogViewerWindow::removeFilter()
{
  int index = removeFilterMenu->actions().indexOf(static_cast<QAction*>(QObject::sender()));
  orpg::LogFilter filter = filters[index];
  filters.removeAt(index);
  if(removeFilterMenu->actions().size()<=3)
  {
    removeFilterMenu->setDisabled(true);
  }
  QObject::sender()->deleteLater();
  emit filterRemoved(filter);
}

void LogViewerWindow::startedProvider()
{
  itemLogMap.clear();
  fileList->clear();
}

void LogViewerWindow::finishedProvider()
{

}

void LogViewerWindow::addLog(QSharedPointer<orpg::JsonLogParser> log)
{
  QTreeWidgetItem* item = getTreeWidgetItem(log);
  fileList->insertTopLevelItem(fileList->topLevelItemCount(),item);
}
