#include "treewidgetextended.h"
#include <QMimeData>

bool TreeWidgetExtended::getPreventDropping() const
{
  return preventDropping;
}

void TreeWidgetExtended::setPreventDropping(bool value)
{
  preventDropping = value;
}

TreeWidgetExtended::TreeWidgetExtended(QWidget *parent) : QTreeWidget(parent)
{
  preventDropping = false;
}

void TreeWidgetExtended::dropEvent(QDropEvent *event)
{
  if(!preventDropping)
  {
    QTreeWidget::dropEvent(event);
    emit itemDropped();
  }
  else
  {
    QByteArray encoded = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&encoded, QIODevice::ReadOnly);
    while (!stream.atEnd())
    {
        int row, col;
        QMap<int,  QVariant> roleDataMap;
        stream >> row >> col >> roleDataMap;
    }
    QTreeWidget::dropEvent(event);
    //event->setDropAction(Qt::IgnoreAction);
  }
}
