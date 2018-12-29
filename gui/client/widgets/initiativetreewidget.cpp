#include "initiativetreewidget.h"

#include <QMimeData>
#include <QDropEvent>
#include <QModelIndex>

InitiativeTreeWidget::InitiativeTreeWidget(QWidget *parent) : QTreeWidget(parent)
{

}

void InitiativeTreeWidget::dropEvent(QDropEvent *event)
{
  DropIndicatorPosition dropIndicator = dropIndicatorPosition();
  QByteArray encoded = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
  QDataStream stream(&encoded, QIODevice::ReadOnly);
  QModelIndex index = indexAt(event->pos());
  int position = index.row();
  int row = -1;
  int col = -1;
  QMap<int,  QVariant> roleDataMap;
  stream >> row >> col >> roleDataMap;
  switch (dropIndicator)
  {
  case QAbstractItemView::AboveItem:
      emit itemDropped(row,position);
      break;
  case QAbstractItemView::BelowItem:
      emit itemDropped(row,position+1);
      break;
  case QAbstractItemView::OnItem:
      break;
  case QAbstractItemView::OnViewport:
      emit itemDropped(row,topLevelItemCount());
      break;
  }
  event->ignore();
}
