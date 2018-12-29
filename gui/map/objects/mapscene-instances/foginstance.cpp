#include "foginstance.h"

#include<QGraphicsRectItem>
#include<QGraphicsItemGroup>
#include<QBrush>
#include<QPen>

QGraphicsRectItem *FogInstance::getItem() const
{
  return item;
}

void FogInstance::setColor(const QColor &color)
{
  item->setBrush(QBrush(color,Qt::SolidPattern));
  item->setPen(QPen(QBrush(color),-1));
}

void FogInstance::setPosition(const QPointF &position)
{
  resource.setCenter(position);
  item->setPos(position);
}

QPointF FogInstance::getPosition() const
{
  return resource.getCenter();
}

MapFogResource FogInstance::getResource() const
{
  return resource;
}

QGraphicsRectItem *FogInstance::generateItem(const MapFogResource &resource, qreal gridSize)
{
  QGraphicsRectItem* item;
  QPointF offset(gridSize/4,gridSize/4);
  item = new QGraphicsRectItem(QRectF(resource.getCenter()*gridSize-offset,resource.getCenter()*gridSize+offset));
  return item;
}

FogInstance::FogInstance()
{
  item = nullptr;
}

FogInstance::FogInstance(const MapFogResource &resource, QGraphicsItemGroup *itemGroup, qreal gridSize, const QColor& color, const QString &tooltip)
{
  this->resource = resource;
  item = generateItem(resource,gridSize);
  item->setToolTip(tooltip);
  setColor(color);
  itemGroup->addToGroup(item);
}

void FogInstance::clear()
{
  item->group()->removeFromGroup(item);
  delete item;
}
