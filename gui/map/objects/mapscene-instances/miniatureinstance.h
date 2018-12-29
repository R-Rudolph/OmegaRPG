#ifndef MINIATUREINSTANCE_H
#define MINIATUREINSTANCE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsTextItem>
#include <QPointF>

#include "core/multi/roomresources/mapresources/mapminiatureresource.h"
#include "gui/client/objects/externalpixmapitemhandler.h"

class MiniatureInstance
{
  MapMiniatureResource miniature;
  QGraphicsItem* item;
  QGraphicsTextItem* nameItem;
  QGraphicsPolygonItem* directionArrowItem;
  QGraphicsLineItem* prevPointLine;
  QPointF prevPos;
  qreal gridSize;
  QPointF getVector(qreal deg);
  void updateNamePosition();
  void moveItemRelative(QGraphicsItem* item, const QPointF& currentPos, const QPointF& nextPos);
  void removeItem(QGraphicsItem* item);
public:
  MiniatureInstance();
  MiniatureInstance(const MapMiniatureResource& miniature, QGraphicsItemGroup* nameItemGroup, ExternalPixmapItemHandler* ih, qreal gridSize, bool showInvisible);
  MiniatureInstance(const MapMiniatureResource& miniature, QGraphicsItemGroup* nameItemGroup, ExternalPixmapItemHandler* ih, qreal gridSize, bool showInvisible, const QPointF& prevPos);
  QString getError();
  QGraphicsItem *getItem() const;
  QGraphicsPolygonItem *getDirectionArrayItem() const;
  MapMiniatureResource getMiniature() const;
  void addToGroup(QGraphicsItemGroup* group);
  void updateGraphicsItems(ExternalPixmapItemHandler* ih);
  QList<QGraphicsItem*> clear();
  void setPreviousPos(const QPointF& pos, qreal gridSize);
  QPointF getPreviousPos(const QPointF& current);
  void setInfoHidden(bool value, int fontSize, bool ignoreSettings=false);
  QGraphicsTextItem *getNameItem() const;
  void setNameTextColor(const QColor& color);
  void movePos(const QPointF& pos);
  void resetPrevPointLine();
  void setShowInvisible(bool showInvisibile);
};

#endif // MINIATUREINSTANCE_H
