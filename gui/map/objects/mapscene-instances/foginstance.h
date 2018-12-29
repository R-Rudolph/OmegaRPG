#ifndef FOGINSTANCE_H
#define FOGINSTANCE_H

#include<QColor>
#include<QPointF>
#include "core/multi/roomresources/mapresources/mapfogresource.h"

class QGraphicsRectItem;
class QGraphicsItemGroup;

class FogInstance
{
  MapFogResource resource;
  QGraphicsRectItem* item;
  QGraphicsRectItem* generateItem(const MapFogResource& resource, qreal gridSize);
public:
  FogInstance();
  FogInstance(const MapFogResource& resource, QGraphicsItemGroup* itemGroup, qreal gridSize, const QColor& color, const QString& tooltip);
  void clear();
  QGraphicsRectItem *getItem() const;
  void setColor(const QColor& color);
  void setPosition(const QPointF& position);
  QPointF getPosition() const;
  MapFogResource getResource() const;
};

#endif // FOGINSTANCE_H
