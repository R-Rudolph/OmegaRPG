#ifndef PLUGINLIBRARYWIDGET_H
#define PLUGINLIBRARYWIDGET_H

#include <QObject>
#include <QMainWindow>
#include <QWidget>
#include <QTreeWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "gui/client/plugins/pluginlib.h"
#include "gui/client/plugins/plugininfowidget.h"
#include "gui/client/plugins/pluginimportinfo.h"
#include<quazip/quazip.h>
#include<QTimer>

class PluginLibraryWidget : public QMainWindow
{
  Q_OBJECT
  QHBoxLayout* layout_;
  QVBoxLayout* leftLayout_;
  QTreeWidget* pluginSelection_;
  PluginInfoWidget* pluginInfo_;
  QPushButton* refreshButton_;

  enum TreeFields
  {
    FIELD_NAME,
    FIELD_DIR
  };

  QTimer* reloadTimer_;

  QTreeWidgetItem* createPluginInfoTreeWidgetItem(QString dirname, const PluginInfo& info);
  QList<PluginImportInfo> getPluginCandidates(QuaZip& zip) const;

  QStringList getCurrentPluginDirs() const;

  void importPluginFile(const QString& filename);
  void importPluginFile(const QStringList& plugins, QuaZip& zip);
  void copy(QIODevice* src, QIODevice* dst, qint64 bufferSize=4000000);

  QStringList getPluginFiles(const QStringList& plugins);
  QStringList getPluginFiles(const QString& directory);
public:
  explicit PluginLibraryWidget(QWidget *parent = nullptr);
protected:
  virtual void 	dragEnterEvent(QDragEnterEvent *event);
  virtual void 	dropEvent(QDropEvent *event);

signals:

public slots:
  void currentSelectionChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
  void reload();
  void importPluginFileSlot();
  void exportPlugin();
  void pluginSelectionContextMenu(const QPoint& pos);
};

#endif // PLUGINLIBRARYWIDGET_H
