#include "pluginlibrarywidget.h"
#include "gui/util/globalgui.h"
#include<QMenuBar>
#include<QFileDialog>
#include<QMessageBox>
#include "gui/client/plugins/pluginimportdialog.h"
#include<QMimeType>
#include<QMimeData>
#include <quazip/quazipfile.h>

QTreeWidgetItem*PluginLibraryWidget::createPluginInfoTreeWidgetItem(QString dirname, const PluginInfo& info)
{
  QTreeWidgetItem* item = new QTreeWidgetItem({dirname,info.name()});
  item->setText(FIELD_NAME, info.name());
  item->setText(FIELD_DIR,  dirname);
  return item;
}

QList<PluginImportInfo> PluginLibraryWidget::getPluginCandidates(QuaZip& zip) const
{
  QList<PluginImportInfo> candidates;
  QStringList candidateDirs;
  QStringList filenames = zip.getFileNameList();
  QList<QuaZipFileInfo> fileinfos = zip.getFileInfoList();
  foreach(QuaZipFileInfo fileinfo, fileinfos)
  {
    const QString& name = fileinfo.name;
    if(fileinfo.name.endsWith("plugin.qml") &&
       (fileinfo.name.count('/')==1))
    {
      candidateDirs.append(name.chopped(name.size()-name.lastIndexOf('/')));
    }
  }
  foreach(QString candidate, candidateDirs)
  {
    PluginImportInfo newPlugin;
    newPlugin.setDirectoryName(candidate);
    foreach(QuaZipFileInfo fileinfo, fileinfos)
    {
      if(fileinfo.name.startsWith(candidate))
      {
        newPlugin.setSizeUncompressed(newPlugin.sizeUncompressed()+fileinfo.uncompressedSize);
      }
    }
    candidates.push_back(newPlugin);
  }
  return candidates;
}

QStringList PluginLibraryWidget::getCurrentPluginDirs() const
{
  QStringList dirs;
  for(int i=0;i<pluginSelection_->topLevelItemCount();i++)
  {
    dirs.append(pluginSelection_->topLevelItem(i)->text(FIELD_DIR));
  }
  return dirs;
}

PluginLibraryWidget::PluginLibraryWidget(QWidget *parent) : QMainWindow(parent,Qt::Window)
{
  setAcceptDrops(true);
  QWidget* centralWidget = new QWidget(this);
  centralWidget->setContentsMargins(0,0,0,0);
  setCentralWidget(centralWidget);
  layout_ = new QHBoxLayout(centralWidget);
  leftLayout_ = new QVBoxLayout();
  pluginSelection_ = new QTreeWidget(this);
  pluginInfo_ = new PluginInfoWidget(this);
  refreshButton_ = new QPushButton(GlobalGUI::refreshIcon(),"");
  layout_->addLayout(leftLayout_);
  leftLayout_->addWidget(pluginSelection_);
  leftLayout_->addWidget(refreshButton_);
  layout_->addWidget(pluginInfo_);
  reload();

  reloadTimer_ = new QTimer(this);
  reloadTimer_->setInterval(500);
  reloadTimer_->setSingleShot(true);
  connect(reloadTimer_,&QTimer::timeout,PluginLib::get(),&PluginLib::reload);

  refreshButton_->setToolTip("Refresh");
  pluginSelection_->setContextMenuPolicy(Qt::CustomContextMenu);
  pluginSelection_->setSelectionMode(QAbstractItemView::ExtendedSelection);
  pluginSelection_->setRootIsDecorated(false);
  pluginSelection_->headerItem()->setText(FIELD_DIR,"Directory");
  pluginSelection_->headerItem()->setText(FIELD_NAME,"Name");

  connect(PluginLib::get(),&PluginLib::reloaded,this,&PluginLibraryWidget::reload);
  connect(pluginSelection_,&QTreeWidget::customContextMenuRequested,this,&PluginLibraryWidget::pluginSelectionContextMenu);
  connect(pluginSelection_,&QTreeWidget::currentItemChanged,this,&PluginLibraryWidget::currentSelectionChanged);
  connect(refreshButton_,&QPushButton::clicked,PluginLib::get(),&PluginLib::reload);

  // menu bar stuff
  QMenu* importMenu = menuBar()->addMenu("Import");
  importMenu->addAction("From File",this,&PluginLibraryWidget::importPluginFileSlot);
}

void PluginLibraryWidget::dragEnterEvent(QDragEnterEvent* event)
{
  if(event->mimeData()->urls().size()==1)
  {
    QString filepath = event->mimeData()->urls().first().toLocalFile();
    if(filepath.endsWith(".zip"))
    {
      event->acceptProposedAction();
    }
  }
}

void PluginLibraryWidget::dropEvent(QDropEvent* event)
{
  if(event->mimeData()->urls().size()==1)
  {
    QString filepath = event->mimeData()->urls().first().toLocalFile();
    if(filepath.endsWith(".zip"))
    {
      importPluginFile(filepath);
    }
  }
}

void PluginLibraryWidget::currentSelectionChanged(QTreeWidgetItem* current, QTreeWidgetItem*)
{
  if(current!=nullptr)
  {
    pluginInfo_->setInfo(PluginLib::get()->pluginInfoByDirectory(current->text(FIELD_DIR)));
  }
  else
  {
    pluginInfo_->setInfo(PluginInfo());
  }
}

void PluginLibraryWidget::reload()
{
  pluginSelection_->clear();
  QMap<QString,PluginInfo> plugins = PluginLib::get()->plugins();
  for(auto it = plugins.begin();it!=plugins.end();it++)
  {
    pluginSelection_->addTopLevelItem(createPluginInfoTreeWidgetItem(it.key(),it.value()));
  }
}

void PluginLibraryWidget::importPluginFileSlot()
{
  QString filename = QFileDialog::getOpenFileName(this,"Select plugin zip file",QString(),"*.zip");
  importPluginFile(filename);
}

void PluginLibraryWidget::importPluginFile(const QString& filename)
{
  if(filename.isEmpty())
    return;
  QuaZip zip(filename);
  if(zip.open(QuaZip::mdUnzip))
  {
    QList<PluginImportInfo> candidates = getPluginCandidates(zip);
    if(candidates.size()>0)
    {
      PluginImportDialog dialog(candidates,getCurrentPluginDirs(),this);
      if(dialog.exec())
      {
        QStringList selectedPlugins = dialog.getSelectedPlugins();
        importPluginFile(selectedPlugins,zip);
        reloadTimer_->start();
      }
    }
    else
    {
      QMessageBox::critical(this,"Import Error","File does not seem to contain plugins.");
    }
    zip.close();
  }
  else
  {
    QMessageBox::critical(this,"Import Error","Could not open zip file.");
  }
}

void PluginLibraryWidget::importPluginFile(const QStringList& plugins, QuaZip& zip)
{
  QDir pluginDir = QDir(Global::pluginsFolder());
  QStringList filelist = zip.getFileNameList();
  foreach(const QString pluginShort, plugins)
  {
    QString plugin = pluginShort+"/";
    /// make dirs
    {
      QSet<QString> dirs;
      foreach(const QString& filename, filelist)
      {
        if(filename.startsWith(plugin) &&
           filename.contains('/'))
        {
          int last = filename.lastIndexOf('/');
          dirs.insert(filename.chopped(filename.size()-last));
        }
      }
      foreach(const QString& dirname, dirs)
      {
        QDir().mkpath(pluginDir.absoluteFilePath(dirname));
      }
    }
    /// write files
    foreach(const QString& filename, filelist)
    {
      if(filename.startsWith(plugin) &&
         !filename.endsWith("/"))
      {
        QFile file(pluginDir.absoluteFilePath(filename));
        if(file.open(QIODevice::WriteOnly))
        {
          zip.setCurrentFile(filename);
          QuaZipFile z(&zip);
          z.open(QIODevice::ReadOnly);
          copy(&z,&file);
          z.close();
          file.close();
        }
        else
        {
          QMessageBox::critical(this,"Import Error",QString("Could not write to file:\n%1").arg(pluginDir.absoluteFilePath(filename)));
          return;
        }
      }
    }
  }
}

void PluginLibraryWidget::copy(QIODevice* src, QIODevice* dst, qint64 bufferSize)
{
  char* buffer = static_cast<char*>(malloc(static_cast<size_t>(bufferSize)*sizeof(char)));
  qint64 readBytes;
  while(src->bytesAvailable()>0)
  {
    readBytes = src->read(buffer,bufferSize);
    dst->write(buffer,readBytes);
  }
  free(buffer);
}

QStringList PluginLibraryWidget::getPluginFiles(const QStringList& plugins)
{
  QStringList result;
  foreach(const QString& plugin, plugins)
  {
    result.append(getPluginFiles(plugin));
  }
  return result;
}

QStringList PluginLibraryWidget::getPluginFiles(const QString& plugin)
{
  QStringList result;
  QDir pluginDir(QDir(Global::pluginsFolder()).absoluteFilePath(plugin));
  QFileInfoList infos = pluginDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
  foreach(const QFileInfo& info, infos)
  {
    if(info.isFile())
    {
      if(!info.fileName().endsWith(".qmlc"))
      {
        result.append(plugin+"/"+info.fileName());
      }
    }
    else if(info.isDir())
    {
      result.append(plugin+"/"+info.fileName()+"/");
    }
  }
  return result;
}

void PluginLibraryWidget::exportPlugin()
{
  auto items = pluginSelection_->selectedItems();
  if(items.size()==0)
    return;
  QStringList plugins;
  foreach(auto item, items)
    plugins.push_back(item->text(FIELD_DIR));
  QString suggestedName;
  if(plugins.size()==1)
    suggestedName = plugins.first()+".zip";
  else
    suggestedName = "plugins.zip";
  QString filename = QFileDialog::getSaveFileName(this,"Export",suggestedName,"*.zip");
  if(filename.isEmpty())
    return;
  QuaZip file(filename);
  if(!file.open(QuaZip::mdCreate))
  {
    QMessageBox::critical(this,"Import Error",QString("Could not write to file:\n%1").arg(filename));
    return;
  }
  QStringList srcfiles = getPluginFiles(plugins);
  QDir pluginDir(Global::pluginsFolder());
  foreach(const QString srcfile, srcfiles)
  {
    QuaZipFile fileWriter(&file);
    if(!srcfile.endsWith('/'))
    {
      if(fileWriter.open(QIODevice::WriteOnly,QuaZipNewInfo(srcfile)))
      {
        QFile readFile(pluginDir.absoluteFilePath(srcfile));
        if(readFile.open(QIODevice::ReadOnly))
        {
          copy(&readFile,&fileWriter);
        }
        else
        {
          QMessageBox::critical(this,"Export Error",QString("Could not read file:\n%1").arg(srcfile));
          return;
        }
        fileWriter.close();
      }
    }
    else
    {
      QMessageBox::critical(this,"Export Error",QString("Could not write to zip file."));
      return;
    }
  }
}

void PluginLibraryWidget::pluginSelectionContextMenu(const QPoint& pos)
{
  auto items = pluginSelection_->selectedItems();
  if(items.size()==0)
    return;
  QMenu menu;
  menu.addAction("Export",this,&PluginLibraryWidget::exportPlugin);
  menu.exec(pluginSelection_->mapToGlobal(pos));
}
