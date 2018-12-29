#ifndef PLUGINIMPORTDIALOG_H
#define PLUGINIMPORTDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include "gui/client/plugins/pluginimportinfo.h"

class PluginImportDialog : public QDialog
{
  Q_OBJECT
  QVBoxLayout* layout_;
  QHBoxLayout* buttonLayout_;
  QTreeWidget* tree_;
  QPushButton* importButton_;
  QPushButton* cancelButton_;

  enum Columns
  {
    ColumnName = 0,
    ColumnSize = 1,
    ColumnAlreadyExists = 2
  };

public:
  explicit PluginImportDialog(const QList<PluginImportInfo>& importInfos,
                              const QStringList& currentPlugins,
                              QWidget *parent = nullptr);
  QStringList getSelectedPlugins() const;

signals:

public slots:
};

#endif // PLUGINIMPORTDIALOG_H
