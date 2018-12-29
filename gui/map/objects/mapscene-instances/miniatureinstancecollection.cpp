#include "miniatureinstancecollection.h"
#include <QTimer>

QGraphicsItemGroup *MiniatureInstanceCollection::getGroupBackground() const
{
  return groupBackground;
}

QGraphicsItemGroup *MiniatureInstanceCollection::getGroupLow() const
{
  return groupLow;
}

QGraphicsItemGroup *MiniatureInstanceCollection::getGroupHigh() const
{
  return groupHigh;
}

QGraphicsItemGroup *MiniatureInstanceCollection::getGroupHighest() const
{
  return groupHighest;
}

void MiniatureInstanceCollection::setOutlineColor(const QColor &color)
{
  if(color != visibleColor)
  {
    visibleColor = color;
    foreach(int i, minis.keys())
    {
      MiniatureInstance& mini = minis.operator [](i);
      if(mini.getMiniature().getDisplay()==MapMiniatureResource::DISPLAY_CIRCLED)
      {
        GraphicsCircledEffect* effect = static_cast<GraphicsCircledEffect*>(mini.getItem()->graphicsEffect());
        if(effect!=nullptr)
        {
          effect->setFrameColor(visibleColor);
        }
      }
      mini.setNameTextColor(color);
    }
  }
}

void MiniatureInstanceCollection::grabItem(const QPointF &pos, const QList<QGraphicsItem *> &items, bool noBackground, bool fogBlocking)
{
  MiniatureInstance* instance = getItemByList(items,&grabbedDirectionMarker,noBackground,fogBlocking);
  if(instance!=nullptr)
  {
    grabbedItem = instance->getMiniature().getID();
    grabbing = true;
  }
  if(grabbing)
  {
    if(grabbedDirectionMarker)
    {
      grabOriginalPos = minis[grabbedItem].getItem()->pos();
      directionLineItem->setLine(QLineF(grabOriginalPos,pos));
      directionLineItem->show();
    }
    else
    {
      grabOriginalPos = minis[grabbedItem].getItem()->pos();
      grabOffset = pos - grabOriginalPos;
    }
  }
}

void MiniatureInstanceCollection::dragItem(const QPointF &pos)
{
  if(!grabbing)
    return;
  if(grabbedDirectionMarker)
  {
    directionLineItem->setLine(QLineF(grabOriginalPos,pos));
  }
  else
  {
    QPointF newPos = getCorrectedPos(pos);
    minis[grabbedItem].movePos(newPos);
  }
}

void MiniatureInstanceCollection::undoGrab()
{
  if(!grabbing)
    return;
  if(grabbedDirectionMarker)
  {
    directionLineItem->hide();
  }
  else
  {
    minis[grabbedItem].movePos(grabOriginalPos);
    minis[grabbedItem].resetPrevPointLine();
  }
  grabbing = false;
}

bool MiniatureInstanceCollection::commitDrag(const QPointF &pos, MapMiniatureResource& result)
{
  if(!grabbing)
    return false;
  result = minis[grabbedItem].getMiniature();
  if(grabbedDirectionMarker)
  {
    result.setDirection(450.0 - QLineF(grabOriginalPos,pos).angle());
    if(result.getDirection()>=360.0)
      result.setDirection(result.getDirection()-360.0);
  }
  else
  {
    QPointF correctedPoint = getCorrectedPos(pos);
    result.setPos(correctedPoint/gridSize);
  }
  undoGrab();
  return true;
}

void MiniatureInstanceCollection::setSnapToGrid(const qreal &value)
{
  snapToGrid = value;
}

bool MiniatureInstanceCollection::showAdditionalInfo(const QList<QGraphicsItem *> &items, float zoom)
{
  bool aboveFog = true;
  int fontSize = qMax(12,(int) (20/zoom) );
  hideCurrentAdditionalInfo();
  if(zoom>30)
    return false;
  if(grabbing)
  {
    prevLineInstance = grabbedItem;
    prevLineSet = true;
    minis[prevLineInstance].setInfoHidden(false, fontSize, true);
  }
  else
  {
    MiniatureInstance* instance = getItemByList(items);
    if(instance != nullptr && instance->getMiniature().getLayer()!=MapMiniatureResource::LAYER_BACKGROUND)
    {
      instance->setInfoHidden(false, fontSize);
      prevLineInstance = instance->getMiniature().getID();
      prevLineSet = true;
      aboveFog = instance->getItem()->group() == groupHighest;
    }
  }
  return aboveFog;
}

void MiniatureInstanceCollection::hideCurrentAdditionalInfo()
{
  if(prevLineSet)
  {
    if(minis.contains(prevLineInstance))
    {
      minis[prevLineInstance].setInfoHidden(true, 1);
    }
    prevLineSet = false;
  }
}

void MiniatureInstanceCollection::clearGarbage()
{
  foreach(QGraphicsItem* item, garbage)
    delete item;
  garbage.clear();
}

QGraphicsItemGroup *MiniatureInstanceCollection::getGroupNames() const
{
  return groupNames;
}

bool MiniatureInstanceCollection::getShowInvisible() const
{
  return showInvisible;
}

void MiniatureInstanceCollection::setShowInvisible(bool value)
{
  showInvisible = value;
  foreach(int id, minis.keys())
  {
    minis[id].setShowInvisible(showInvisible);
  }
}

QPointF MiniatureInstanceCollection::getCorrectedPos(QPointF pos)
{
  pos -= grabOffset;
  if(snapToGrid==0.0)
  {
    return pos;
  }
  else
  {
    qreal virtGridSize;
    qreal miniatureSize = minis[grabbedItem].getMiniature().getSize();
    int miniatureSizeInt = miniatureSize;
    qreal gridOffset = 0.5;

    if(miniatureSize>=1.0)
    {
      virtGridSize = gridSize;
      if(miniatureSizeInt%2 == 0)
      {
        gridOffset = 0.0;
      }
    }
    else if(miniatureSize>=0.5)
      virtGridSize = gridSize/2;
    else if(miniatureSize>=0.25)
      virtGridSize = gridSize/4;
    else if(miniatureSize>=0.125)
      virtGridSize = gridSize/8;
    else
      virtGridSize = gridSize;
    pos = QPointF(virtGridSize * (std::floor(pos.x()/virtGridSize)+gridOffset), virtGridSize * (std::floor(pos.y()/virtGridSize)+gridOffset));
    return pos;
  }
}

void MiniatureInstanceCollection::addToGarbage(QList<QGraphicsItem *> garbage)
{
  bool added = false;
  foreach(QGraphicsItem* item, garbage)
  {
    if(item!=nullptr)
      garbage.append(item);
  }
  if(added)
  {
    garbageTimer->start();
  }
}

MiniatureInstanceCollection::MiniatureInstanceCollection(ExternalPixmapItemHandler* ih, qreal gridSize, qreal snapToGrid, QObject *parent)
  :QObject(parent)
{
  showInvisible = true;
  prevLineInstance = 0;
  prevLineSet = false;
  this->snapToGrid = snapToGrid;
  grabbing = false;
  grabbedItem = 0;
  this->gridSize = gridSize;
  this->ih = ih;
  groupNames = new QGraphicsItemGroup;
  groupHighest = new QGraphicsItemGroup;
  groupHigh = new QGraphicsItemGroup;
  groupLow = new QGraphicsItemGroup;
  groupBackground = new QGraphicsItemGroup;
  layerGroupMap.insert(MapMiniatureResource::LAYER_ABOVEFOG,groupHighest);
  layerGroupMap.insert(MapMiniatureResource::LAYER_ABOVELINES,groupHigh);
  layerGroupMap.insert(MapMiniatureResource::LAYER_BELOWLINES,groupLow);
  layerGroupMap.insert(MapMiniatureResource::LAYER_BACKGROUND,groupBackground);

  directionLineItem = new QGraphicsLineItem;
  groupNames->addToGroup(directionLineItem);
  directionLineItem->hide();
  directionLineItem->setPen(QPen(Qt::blue));

  visibleColor = Qt::black;

  garbageTimer = new QTimer(this);
  garbageTimer->setSingleShot(true);
  garbageTimer->setInterval(2000);
  connect(garbageTimer,&QTimer::timeout,this,&MiniatureInstanceCollection::clearGarbage);
}

QGraphicsItem *MiniatureInstanceCollection::addMini(const MapMiniatureResource &mini)
{
  bool replaces = minis.contains(mini.getID());
  QPointF prevPos = mini.getPos();
  MiniatureInstance* oldMini;
  if(replaces)
  {
    oldMini = &(minis[mini.getID()]);
    prevPos = oldMini->getMiniature().getPos();
  }
  MiniatureInstance newInstance(mini,groupNames,ih,gridSize,showInvisible);
  newInstance.setNameTextColor(visibleColor);
  newInstance.setPreviousPos(prevPos,gridSize);
  newInstance.addToGroup(layerGroupMap[mini.getLayer()]);
  if(replaces)
  {
    removeMini(mini.getID(),true);
  }
  minis.insert(mini.getID(),newInstance);
  return newInstance.getItem();
}

MiniatureInstance *MiniatureInstanceCollection::getItemByID(int id)
{
  foreach(int i, minis.keys())
  {
    if(minis[i].getMiniature().getID()==id)
      return &minis[i];
  }
  return nullptr;
}

MiniatureInstance *MiniatureInstanceCollection::getItemByMiniature(const MapMiniatureResource& mini)
{
  return getItemByID(mini.getID());
}

MiniatureInstance *MiniatureInstanceCollection::getItemByGraphicsItem(QGraphicsItem *item)
{
  foreach(int i, minis.keys())
  {
    if(minis[i].getItem()==item || minis[i].getDirectionArrayItem()==item)
      return &minis[i];
  }
  return nullptr;
}

MiniatureInstance *MiniatureInstanceCollection::getItemByList(const QList<QGraphicsItem *> &items, bool *grabbedDirectionMarker, bool noBackground, bool fogBlocking)
{
  for(int i=0;i<items.size();i++)
  {
    foreach(int j, minis.keys())
    {
      if((minis[j].getItem()==items[i] || minis[j].getDirectionArrayItem()==items[i]) &&
         !(minis[j].getMiniature().getLayer()==MapMiniatureResource::LAYER_BACKGROUND && noBackground) &&
         !(minis[j].getMiniature().getLayer()!=MapMiniatureResource::LAYER_ABOVEFOG && fogBlocking))
      {
        if(grabbedDirectionMarker!=nullptr)
          *grabbedDirectionMarker = minis[j].getDirectionArrayItem()==items[i];
        return &minis[j];
      }
    }
  }
  return nullptr;
}

QList<MiniatureInstance *> MiniatureInstanceCollection::getItemsByList(const QList<QGraphicsItem *> &items, bool *grabbedDirectionMarker, bool noBackground, bool fogBlocking)
{
  QList<MiniatureInstance *> result;
  for(int i=0;i<items.size();i++)
  {
    foreach(int j, minis.keys())
    {
      if((minis[j].getItem()==items[i] || minis[j].getDirectionArrayItem()==items[i]) &&
         !(minis[j].getMiniature().getLayer()==MapMiniatureResource::LAYER_BACKGROUND && noBackground) &&
         !(minis[j].getMiniature().getLayer()!=MapMiniatureResource::LAYER_ABOVEFOG && fogBlocking))
      {
        if(grabbedDirectionMarker!=nullptr)
          *grabbedDirectionMarker = minis[j].getDirectionArrayItem()==items[i];
        MiniatureInstance* instance = &minis[j];
        if(!result.contains(instance))
          result.append(instance);
      }
    }
  }
  return result;
}

void MiniatureInstanceCollection::removeMini(int id, bool getsReplaced)
{
  if(minis.contains(id))
  {

    if(!getsReplaced)
    {
      if(grabbing && (grabbedItem==id))
        undoGrab();
      if(prevLineSet && (id==prevLineInstance))
        hideCurrentAdditionalInfo();
    }
    MiniatureInstance instance = minis[id];
    ih->removeGraphicsItem(instance.getItem());
    addToGarbage(instance.clear());
    minis.remove(id);
  }
}

void MiniatureInstanceCollection::clear()
{
  undoGrab();
  foreach(int i, minis.keys())
  {
    ih->removeGraphicsItem(minis[i].getItem());
    minis[i].clear();
  }
  minis.clear();
}
