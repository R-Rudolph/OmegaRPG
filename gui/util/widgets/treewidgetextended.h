#ifndef TREEWIDGETEXTENDED_H
#define TREEWIDGETEXTENDED_H

#include <QWidget>
#include <QTreeWidget>
#include <QDropEvent>

class TreeWidgetExtended : public QTreeWidget
{
  Q_OBJECT
  bool preventDropping;
public:
  explicit TreeWidgetExtended(QWidget *parent = 0);
  bool getPreventDropping() const;
  void setPreventDropping(bool value);

protected:
  virtual void dropEvent(QDropEvent *event);
signals:
  void itemCheckedStateChanged(QTreeWidgetItem* item, Qt::CheckState state);
  void itemDropped();
public slots:
};

#endif // TREEWIDGETEXTENDED_H
