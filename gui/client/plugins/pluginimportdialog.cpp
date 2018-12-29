#include "pluginimportdialog.h"
#include "gui/util/globalgui.h"

PluginImportDialog::PluginImportDialog(const QList<PluginImportInfo>& importInfos,
                                       const QStringList& currentPlugins,
                                       QWidget *parent) : QDialog(parent)
{
  /// set dialog parameter
  setWindowTitle("Import Plugins");
  setModal(true);

  /// intialize layouts and widgets
  layout_ = new QVBoxLayout(this);
  buttonLayout_ = new QHBoxLayout();

  tree_ = new QTreeWidget(this);
  importButton_ = new QPushButton("Import",this);
  cancelButton_ = new QPushButton("Cancel",this);

  layout_->addWidget(tree_);
  layout_->addLayout(buttonLayout_);
  buttonLayout_->addWidget(importButton_);
  buttonLayout_->addWidget(cancelButton_);

  connect(importButton_,&QPushButton::clicked,this,&QDialog::accept);
  connect(cancelButton_,&QPushButton::clicked,this,&QDialog::reject);

  /// fill tree widget
  tree_->headerItem()->setText(ColumnName,"Name");
  tree_->headerItem()->setText(ColumnSize,"Size");
  tree_->headerItem()->setText(ColumnAlreadyExists,"Already Exists");
  tree_->setRootIsDecorated(false);
  tree_->hideColumn(ColumnAlreadyExists);
  foreach(const PluginImportInfo& info, importInfos)
  {
    bool existsAlready = currentPlugins.contains(info.directoryName());
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(ColumnName,info.directoryName());
    item->setText(ColumnSize,info.getHumanReadableSize());
    item->setText(ColumnAlreadyExists,QString::number(existsAlready));
    if(existsAlready)
    {
      item->setIcon(0,GlobalGUI::freedesktopCopy());
      item->setCheckState(0,Qt::Unchecked);
      item->setToolTip(ColumnName,"Plugin already exists.");
      item->setToolTip(ColumnSize,"Plugin already exists.");
    }
    else
    {
      item->setCheckState(0,Qt::Checked);
    }
    tree_->addTopLevelItem(item);
  }
  for(int i = 0; i < tree_->columnCount(); i++)
    tree_->resizeColumnToContents(i);

}

QStringList PluginImportDialog::getSelectedPlugins() const
{
  QStringList selected;
  for(int i = 0; i < tree_->topLevelItemCount(); i++)
  {
    if(tree_->topLevelItem(i)->checkState(0)==Qt::Checked)
      selected.append(tree_->topLevelItem(i)->text(ColumnName));
  }
  return selected;
}
