#ifndef TREEWIDGETITEMEXTENDED_H
#define TREEWIDGETITEMEXTENDED_H

#include <QTreeWidgetItem>
#include "widgets/treewidgetextended.h"

class TreeWidgetItemExtended : public QTreeWidgetItem
{
  void setData(int column, int role, const QVariant& value);
public:
  TreeWidgetItemExtended();
};

#endif // TREEWIDGETITEMEXTENDED_H
