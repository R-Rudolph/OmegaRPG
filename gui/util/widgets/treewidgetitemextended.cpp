#include "treewidgetitemextended.h"

TreeWidgetItemExtended::TreeWidgetItemExtended()
{

}

void TreeWidgetItemExtended::setData(int column, int role, const QVariant& value)
{
  const bool isCheckChange = column == 0
                            && role == Qt::CheckStateRole
                            && data(column, role).isValid() // Don't "change" during initialization
                            && checkState(0) != value;
  QTreeWidgetItem::setData(column, role, value);
  if (isCheckChange)
  {
    TreeWidgetExtended* tree = static_cast<TreeWidgetExtended *>(treeWidget());
    emit tree->itemCheckedStateChanged(this, checkState(0));
  }
}
