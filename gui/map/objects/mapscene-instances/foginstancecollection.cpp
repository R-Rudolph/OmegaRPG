#include "foginstancecollection.h"

#include<QGraphicsItemGroup>

QColor FogInstanceCollection::getFogColor() const
{
  return fogColor;
}

void FogInstanceCollection::setFogColor(const QColor &value)
{
  if(fogColor==value)
    return;
  fogColor = value;
  foreach(FogInstance instance, instanceMap.values())
  {
    instance.setColor(fogColor);
  }
}

void FogInstanceCollection::makeFog(QPointF position, qreal radius)
{
  QList<QPointF> points = getPositions(position, radius);
  QList<MapFogResource> resources;
  foreach(const QPointF& point, points)
    resources.append(point);
  emit fogGenerated(resources);
}

QGraphicsItemGroup *FogInstanceCollection::getItemGroup() const
{
  return groupFog;
}

void FogInstanceCollection::addFogStart(const QPointF &position, qreal radius)
{
  addFogCancel();
  addFogDraw(position,radius);
}

void FogInstanceCollection::addFogDraw(const QPointF &position, qreal radius)
{
  QList<QPointF> points = getPositions(position, radius);
  foreach(const QPointF& point, points)
  {
    if(!addedFog.contains(point) && !positionMap.contains(point))
    {
      addedFog.insert(point,FogInstance(MapFogResource(point),groupFog,gridSize,fogColor,""));
    }
  }
}

void FogInstanceCollection::addFogCommit()
{
  QList<MapFogResource> resources;
  foreach(FogInstance instance, addedFog.values())
  {
    resources.append(instance.getResource());
    instance.clear();
  }
  addedFog.clear();
  emit fogGenerated(resources);
}

void FogInstanceCollection::addFogCancel()
{
  foreach(FogInstance instance, addedFog.values())
  {
    instance.clear();
  }
  addedFog.clear();
}

void FogInstanceCollection::removeFogStart(const QPointF &position, qreal radius)
{
  removeFogCancel();
  removeFogDraw(position,radius);
}

void FogInstanceCollection::removeFogDraw(const QPointF &position, qreal radius)
{
  QList<QPointF> points = getPositions(position, radius);
  foreach(const QPointF& point, points)
  {
    if(positionMap.contains(point))
    {
      int id = positionMap[point];
      if(!removedIndices.contains(id))
      {
        instanceMap[id].getItem()->hide();
        removedIndices.insert(id,id);
      }
    }
  }
}

void FogInstanceCollection::removeFogCommit()
{
  auto temp = removedIndices.keys();
  removeFogCancel();
  emit fogRemoved(temp);
}

void FogInstanceCollection::removeFogCancel()
{
  foreach(int id, removedIndices.keys())
  {
    instanceMap[id].getItem()->show();
  }
  removedIndices.clear();
}

void FogInstanceCollection::setPlayerRole(PlayerRole value)
{
  role = value;
  switch(role)
  {
    case ROLE_GM:
    case ROLE_ADMIN:
    {
      QColor color = fogColor;
      color.setAlpha(100);
      setFogColor(color);
      break;
    }
    default:
    {
      QColor color = fogColor;
      color.setAlpha(255);
      this->setFogColor(color);
      break;
    }
  }
}

bool FogInstanceCollection::hides(QPointF position)
{
  position /= gridSize;
  position *= 2;
  position = QPointF(0.5+qFloor(position.x()),0.5+qFloor(position.y()));
  position /= 2;
  return positionMap.contains(position);
}

void FogInstanceCollection::setTooltip(const QString &value)
{
  if(tooltip==value)
    return;
  tooltip = value;
  foreach(FogInstance instance, instanceMap.values())
  {
    instance.getItem()->setToolTip(tooltip);
  }
}

QList<QPointF> FogInstanceCollection::getPositions(QPointF position, qreal radius)
{
  QList<QPointF> result;
  position /= gridSize/2;
  radius /= gridSize/2;
  qreal radiusSquared = radius*radius;
  for(qreal y=qFloor(-radius+position.y())-1;y<=qCeil(radius+position.y())+1;y++)
  {
    for(qreal x=qFloor(-radius+position.x())-1;x<=qCeil(radius+position.x())+1;x++)
    {
      QPointF pos(x+0.5,y+0.5);
      QPointF diff = pos - position;
      if((diff.x()*diff.x()+diff.y()*diff.y()-radiusSquared)>=0.4)
        continue;
      result.append(pos/2);
    }
  }
  return result;
}

FogInstanceCollection::FogInstanceCollection(qreal gridSize, QObject *parent)
  : QObject(parent)
{
  this->gridSize = gridSize;
  this->groupFog = new QGraphicsItemGroup();
  setFogColor(Qt::gray);
}

void FogInstanceCollection::addFog(const MapFogResource &resource)
{
  if(instanceMap.contains(resource.getID()))
  {
    instanceMap[resource.getID()].setPosition(resource.getCenter());
  }
  else
  {
    instanceMap.insert(resource.getID(),FogInstance(resource,groupFog,gridSize,fogColor,tooltip));
    positionMap.insert(resource.getCenter(),resource.getID());
  }
  //remove from adding
  if(addedFog.contains(resource.getCenter()))
  {
    addedFog[resource.getCenter()].clear();
    addedFog.remove(resource.getCenter());
  }
}

void FogInstanceCollection::removeFog(int id)
{
  if(instanceMap.contains(id))
  {
    FogInstance instance = instanceMap[id];
    instanceMap.remove(id);
    positionMap.remove(instance.getPosition());
    instance.clear();
  }
}
