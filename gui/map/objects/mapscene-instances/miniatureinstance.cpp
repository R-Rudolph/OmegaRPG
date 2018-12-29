#include "miniatureinstance.h"
#include <QGraphicsDropShadowEffect>

QGraphicsItem *MiniatureInstance::getItem() const
{
  return item;
}

QGraphicsPolygonItem *MiniatureInstance::getDirectionArrayItem() const
{
  return directionArrowItem;
}

MapMiniatureResource MiniatureInstance::getMiniature() const
{
  return miniature;
}

void MiniatureInstance::addToGroup(QGraphicsItemGroup *group)
{
  if(prevPointLine)
    group->addToGroup(prevPointLine);
  if(item)
    group->addToGroup(item);
  if(directionArrowItem)
    group->addToGroup(directionArrowItem);
}

QList<QGraphicsItem*> MiniatureInstance::clear()
{
  QList<QGraphicsItem*> result;
  removeItem(prevPointLine);
  removeItem(item);
  removeItem(directionArrowItem);
  removeItem(nameItem);
  result.append(prevPointLine);
  result.append(item);
  result.append(directionArrowItem);
  result.append(nameItem);
  return result;
}

void MiniatureInstance::setPreviousPos(const QPointF &pos, qreal gridSize)
{
  if(pos!=miniature.getPos())
  {
    prevPos = pos;
    prevPointLine->setLine(QLineF(pos*gridSize,miniature.getPos()*gridSize));
  }
}

QPointF MiniatureInstance::getPreviousPos(const QPointF &current)
{
  if(current!=miniature.getPos())
    return miniature.getPos();
  if(prevPointLine!=nullptr)
    return prevPos;
  return miniature.getPos();
}

void MiniatureInstance::setInfoHidden(bool value, int fontSize, bool ignoreSettings)
{
  if(prevPointLine != nullptr)
  {
    if(value || (prevPointLine->line().p1() == prevPointLine->line().p2()) || (!ignoreSettings && !orpg::Settings::get()->showPreviousMiniaturePosLine) )
      prevPointLine->hide();
    else
      prevPointLine->show();
  }
  if(nameItem != nullptr)
  {
    if(value)
    {
      nameItem->hide();
    }
    else
    {
      QFont font;
      font.setPixelSize(fontSize);
      nameItem->setFont(font);
      updateNamePosition();
      nameItem->show();
    }
  }
}

QGraphicsTextItem *MiniatureInstance::getNameItem() const
{
  return nameItem;
}

void MiniatureInstance::setNameTextColor(const QColor &color)
{
  if(nameItem != nullptr)
  {
    nameItem->setDefaultTextColor(color);
    //static_cast<QGraphicsDropShadowEffect*>(nameItem->graphicsEffect())->setColor(QColor(255-color.red(),255-color.green(),255-color.blue()));
  }
}

void MiniatureInstance::movePos(const QPointF &pos)
{
  moveItemRelative(directionArrowItem,item->pos(),pos);
  moveItemRelative(nameItem,item->pos(),pos);
  item->setPos(pos);
  if(nameItem)
  {
    nameItem->setPlainText(orpg::Settings::get()->distanceMeasure.distanceString(miniature.getPos(),pos/gridSize));
    updateNamePosition();
  }
  if(miniature.getPos()*gridSize != pos)
  {
    prevPointLine->setLine(QLineF(miniature.getPos()*gridSize,pos));
  }
  else
  {
    prevPointLine->hide();
    prevPointLine->setLine(QLineF(miniature.getPos()*gridSize,pos));
  }
}

void MiniatureInstance::resetPrevPointLine()
{
  if(nameItem)
    nameItem->setPlainText(miniature.getName());
  prevPointLine->hide();
  prevPointLine->setLine(QLineF(prevPos*gridSize,item->pos()));
}

void MiniatureInstance::setShowInvisible(bool showInvisibile)
{
  if(item!=nullptr)
  {
    switch(miniature.getVisibility())
    {
      case MapMiniatureResource::VISIBILITY_VISIBLE:
        item->show();
        item->setOpacity(1.0);
        break;
      case MapMiniatureResource::VISIBILITY_PARTIAL:
        item->show();
        item->setOpacity(0.7);
        break;
      case MapMiniatureResource::VISIBILITY_INVISIBLE:
        if(showInvisibile)
        {
          item->show();
          item->setOpacity(0.4);
        }
        else
        {
          item->hide();
          item->setOpacity(1.0);
        }
        break;
    }
  }
}

QPointF MiniatureInstance::getVector(qreal deg)
{
  return QPointF(qCos(deg*M_PI/180),qSin(deg*M_PI/180));
}

void MiniatureInstance::updateNamePosition()
{
  QPointF pos = item->pos();
  pos.ry() += miniature.getSize()*gridSize/2;
  pos.rx() -= nameItem->boundingRect().width()/2;
  nameItem->setPos(pos);
}

void MiniatureInstance::moveItemRelative(QGraphicsItem *item, const QPointF &currentPos, const QPointF &nextPos)
{
  if(item)
  {
    item->setPos(item->pos() + (nextPos - currentPos));
  }
}

void MiniatureInstance::removeItem(QGraphicsItem *item)
{
  if(item)
  {
    item->group()->removeFromGroup(item);
    item->setVisible(false);
    //delete item;
  }
}

MiniatureInstance::MiniatureInstance()
{

}

MiniatureInstance::MiniatureInstance(const MapMiniatureResource &miniature, QGraphicsItemGroup* nameItemGroup, ExternalPixmapItemHandler* ih, qreal gridSize, bool showInvisible)
{
  item = nullptr;
  directionArrowItem = nullptr;
  prevPointLine = new QGraphicsLineItem();
  prevPointLine->setOpacity(0.5);
  prevPointLine->setPen(QPen(Qt::red,1,Qt::DashLine));
  prevPointLine->hide();
  prevPos = miniature.getPos();
  nameItem = nullptr;
  this->gridSize = gridSize;
  this->miniature = miniature;
  if(!miniature.getGraphic().isEmpty())
  {
    item = ih->getGraphicsItem(miniature.getGraphic(),miniature.getSize()*gridSize,miniature.getDisplay());
    item->setRotation(miniature.getRotation());
    item->setPos(miniature.getPos()*gridSize);

    if(this->miniature.getDirection()>=0.0)
    {
      qreal direction = this->miniature.getDirection() - 90;
      QPointF center = miniature.getPos()*gridSize;
      QPolygonF polygon;
      int width = qMax(0.0,miniature.getSize()*0.5);
      polygon.push_back(center+getVector(direction)*(miniature.getSize()*gridSize-width)*0.5);
      polygon.push_back(center+getVector(direction-20.0)*miniature.getSize()*gridSize*0.3);
      polygon.push_back(center+getVector(direction+20.0)*miniature.getSize()*gridSize*0.3);
      directionArrowItem = new QGraphicsPolygonItem(polygon);
      directionArrowItem->setBrush(QBrush(Qt::blue));
      directionArrowItem->setPen(QPen(QBrush(Qt::black),width));
    }
    if(!miniature.getName().isEmpty())
    {
      nameItem = new QGraphicsTextItem(miniature.getName(),nullptr);
      QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
      effect->setOffset(0.0);
      effect->setBlurRadius(30);
      effect->setColor(Qt::white);
      nameItem->setGraphicsEffect(effect);
      updateNamePosition();
      nameItem->hide();
      nameItemGroup->addToGroup(nameItem);
    }
  }
  setShowInvisible(showInvisible);
}

MiniatureInstance::MiniatureInstance(const MapMiniatureResource &miniature, QGraphicsItemGroup* nameItemGroup, ExternalPixmapItemHandler *ih, qreal gridSize, bool showInvisible, const QPointF &prevPos)
  :MiniatureInstance(miniature,nameItemGroup,ih,gridSize,showInvisible)
{
  setPreviousPos(prevPos, gridSize);
}

QString MiniatureInstance::getError()
{
  return item->data(0).toString();
}
