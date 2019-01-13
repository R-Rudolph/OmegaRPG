#include "mapscene.h"
#include <QGuiApplication>
#include <QGraphicsView>
#include <QClipboard>
#include "core/client/clientcore.h"
#include "core/client/clientcontrol.h"
#include "core/multi/roomresources/initiativeresource.h"
#include "core/multi/roomresources/genericresource.h"

#include "core/multi/roomresources/mapresources/maplineresource.h"
#include "core/multi/roomresources/mapresources/maplineresourcecontainer.h"
#include "core/multi/roomresources/mapresources/mapfogresource.h"
#include "core/multi/roomresources/mapresources/mapfogresourcecontainer.h"
#include "core/multi/roomresources/mapresources/maptextresource.h"
#include "core/multi/roomresources/mapresources/maptextresourcecontainer.h"
#include "core/multi/roomresources/mapresources/mapminiatureresource.h"
#include "core/multi/roomresources/mapresources/mapminiatureresourcecontainer.h"
#include "core/multi/roomvariables/mapsettingsvariable.h"
#include "core/multi/message/roomresourcemessage.h"

qreal euclidDistance(const QPointF& point1, const QPointF& point2)
{
  QPointF diff = point1 - point2;
  return sqrt(diff.x()*diff.x()+diff.y()*diff.y());
}

orpg::Message MapScene::lineMessagesFromVector(const QVector<QPointF> &points, int width, const QString &color)
{
  QList<GenericResource> resources;
  for(int i=0;i<points.size()-1;i++)
  {
    resources.append(MapLineResource(QLineF(points[i],points[i+1]),width,color));
  }
  return orpg::RoomResourceMessage::newResourceMessage(resources);
}

void MapScene::updateBackground()
{
  if(!nightMode)
    setBackgroundBrush(QBrush(backgroundColor));
  else
  {
    QColor bgColor = backgroundColor;
    bgColor.setRed(bgColor.red()*0.376470588);
    bgColor.setGreen(bgColor.green()*0.376470588);
    bgColor.setBlue(bgColor.blue()*0.376470588);
    setBackgroundBrush(QBrush(bgColor));
  }
}

void MapScene::setCanvasSize(qreal x1, qreal y1, qreal x2, qreal y2)
{
  this->x1 = x1;
  this->x2 = x2;
  this->y1 = y1;
  this->y2 = y2;
  if(this->x1>this->x2)
    std::swap(x1,x2);
  if(this->y1>this->y2)
    std::swap(y1,y2);
  setSceneRect(x1,y1,x2,y2);
}

void MapScene::drawGrid()
{
  qreal x = this->x1;
  while(x <= x2)
  {
    gridItems.insert(addLine(x,y1,x,x2,QPen(QColor(Qt::lightGray),0,Qt::DashLine)));
    x += gridSize;
  }
  qreal y = this->y1;
  while(y <= y2)
  {
    gridItems.insert(addLine(x1,y,x2,y,QPen(QColor(Qt::lightGray),0,Qt::DashLine)));
    y += gridSize;
  }
}

void MapScene::drawGrid(int x, int y, bool hideGrid)
{
  foreach(QGraphicsItem* item, gridItems)
  {
    gridGroup->removeFromGroup(item);
    this->removeItem(item);
    delete item;
  }
  gridItems.clear();

  int xmin = -x/2;
  int xmax = x/2 + x%2;
  int ymin = -y/2;
  int ymax = y/2 + y%2;
  setSceneRect((xmin-5)*gridSize,(ymin-5)*gridSize,(x+10)*gridSize,(y+10)*gridSize);
  if(hideGrid)
    return;
  if(x>0 && y>0)
  {
    for(int i=xmin;i<=xmax;i++)
    {
      QGraphicsItem* item = addLine(i*gridSize,ymin*gridSize,i*gridSize,ymax*gridSize,QPen(QColor(Qt::lightGray),0,Qt::DashLine));
      gridGroup->addToGroup(item);
      gridItems.insert(item);
    }
    for(int i=ymin;i<=ymax;i++)
    {
      QGraphicsItem* item = addLine(xmin*gridSize,i*gridSize,xmax*gridSize,i*gridSize,QPen(QColor(Qt::lightGray),0,Qt::DashLine));
      gridGroup->addToGroup(item);
      gridItems.insert(item);
    }
  }
}

void MapScene::colorGrid(const QColor &color)
{
  foreach(QGraphicsItem* item, gridItems)
  {

    ((QGraphicsLineItem*)item)->setPen(QPen(color,0,Qt::DashLine));
  }
}

void MapScene::cancelDrawing()
{
  switch(currentTool)
  {
    case TOOL_PENCIL:
      for(int i=0;i<previewItems.size();i++)
      {
        this->removeItem(previewItems[i]);
        delete previewItems[i];
      }
      previewItems.clear();
      pointVec.clear();
      break;

    case TOOL_ERASER:
      foreach(QGraphicsItem* value, lineItemSetEdit)
        value->show();
      lineItemSetEdit.clear();
      break;

    case TOOL_LINE:
      clearToolPreview();
      break;

    case TOOL_RECTANGLE:
      clearToolPreview();
      break;

    case TOOL_FOG:
      fog->addFogCancel();
      break;

    case TOOL_CLEARFOG:
      fog->removeFogCancel();
      break;

    case TOOL_MINIATURE:
      clearMiniaturePreview();
      break;

    case TOOL_POINTER:
      resetMiniaturePos();
      break;

    case TOOL_MEASURE:
      clearMeasurement();
      break;

    default:
      break;
  }
  drawing = false;
}

void MapScene::drawPoint(QPointF point)
{
  switch(currentTool)
  {
    case TOOL_PENCIL:
      if(euclidDistance(lastPoint,point) > distanceThreshold)
      {
        previewItems.push_back((QGraphicsItem*)addLine(QLineF(lastPoint,point),getPen()));

        lastPoint = point;
        pointVec.push_back(lastPoint);
      }
      break;

    case TOOL_ERASER:
      {
        QGraphicsEllipseItem circle(point.x()-getEraserSize(),point.y()-getEraserSize(),2*getEraserSize(),2*getEraserSize());
        QList<QGraphicsItem*> coll = this->collidingItems(&circle);
        for(QList<QGraphicsItem*>::Iterator i = coll.begin();i!=coll.end();i++)
        {
          QGraphicsLineItem* item = (QGraphicsLineItem*) *i;
          if(lineItemSet.contains(item))
          {
            lineItemSetEdit.insert(item);
            item->hide();
          }
        }
        break;
      }

    case TOOL_LINE:
      clearToolPreview();
      toolPreview = this->addLine(QLineF(lastPoint,point),getPen());
      break;

    case TOOL_RECTANGLE:
      {
        clearToolPreview();
        QRectF rect(std::min(lastPoint.x(),point.x()),std::min(lastPoint.y(),point.y()),std::abs(lastPoint.x()-point.x()),std::abs(lastPoint.y()-point.y()));
        toolPreview = this->addRect(rect,getPen());
        break;
      }

    case TOOL_FOG:
      fog->addFogDraw(point,getEraserSize());
      break;

    case TOOL_CLEARFOG:
      fog->removeFogDraw(point,getEraserSize());
      break;

    case TOOL_MINIATURE:
      drawMiniaturePreview(point.x(),point.y());
      break;

    case TOOL_POINTER:
      moveMiniature(point);
      break;

    case TOOL_MEASURE:
      drawMeasurement(point);
      break;

    default:
      break;
  }
}

void MapScene::drawInit(QPointF point)
{
  switch(currentTool)
    {
    case TOOL_PENCIL:
      pointVec.clear();
      lastPoint = point;
      pointVec.push_back(point);
      break;

    case TOOL_ERASER:
      lineItemSetEdit.clear();
      drawPoint(point);
      break;

    case TOOL_LINE:
      pointVec.clear();
      lastPoint = point;
      break;

    case TOOL_RECTANGLE:
      pointVec.clear();
      lastPoint = point;
      break;

    case TOOL_FOG:
      fog->addFogStart(point,getEraserSize());
      break;

    case TOOL_CLEARFOG:
      fog->removeFogStart(point,getEraserSize());
      break;

    case TOOL_MINIATURE:
      drawMiniaturePreview(point.x(),point.y());
      break;

    case TOOL_POINTER:
      grabMiniature(point);
      break;

    case TOOL_TEXT:
    {
      MapTextDialog dialog;
      if(dialog.exec())
      {
        int size = dialog.getSize();
        MapTextResource resource(dialog.getText(),brushColor.name(),size,point);
        sendMessage(orpg::RoomResourceMessage::newResourceMessage(resource));
        //sendMessage(orpg::ClientCore::get()->getResources()->getMapTextContainer()->newMessage(resource));
      }
      break;
    }
    case TOOL_MEASURE:
    {
      measureStart = point;
      break;
    }
    default:
      break;
    }
}

void MapScene::drawCommit(QPointF point)
{
  switch(currentTool)
  {
    case TOOL_PENCIL:
      if(lastPoint != point)
      {
        lastPoint = point;
        pointVec.push_back(lastPoint);
      }
      dividePointVector(pointVec);
      sendMessage(lineMessagesFromVector(pointVec,brushSize,brushColor.name()));
      break;

    case TOOL_ERASER:
      {
        QList<int> indices;
        foreach(QGraphicsItem* value, lineItemSetEdit)
        {
          indices.append(value->data(0).toInt());
        }
        sendMessage(orpg::RoomResourceMessage::removeResourceMessage(MapLineResource::resourceName,indices));
        //sendMessage(orpg::ClientCore::get()->get()->getResources()->getMapLineContainer()->removeMessage(indices));
        cancelDrawing();
        break;
      }

    case TOOL_RECTANGLE:
      if(lastPoint.x() != point.x() && lastPoint.y() != point.y())
      {
        pointVec.push_back(lastPoint);
        pointVec.push_back(QPointF(lastPoint.x(),point.y()));
        pointVec.push_back(point);
        pointVec.push_back(QPointF(point.x(),lastPoint.y()));
        pointVec.push_back(lastPoint);
        dividePointVector(pointVec);
        sendMessage(lineMessagesFromVector(pointVec,brushSize,brushColor.name()));
      }
      break;

    case TOOL_LINE:
      if(lastPoint != point)
      {
        pointVec.push_back(lastPoint);
        pointVec.push_back(point);
        dividePointVector(pointVec);
        sendMessage(lineMessagesFromVector(pointVec,brushSize,brushColor.name()));
      }
      break;

    case TOOL_CLEARFOG:
      fog->removeFogCommit();
      break;

    case TOOL_FOG:
      fog->addFogCommit();
      break;

    case TOOL_MINIATURE:
    {
      clearMiniaturePreview();
      miniature.setPos(point/gridSize);
      newMiniature(miniature);
      break;
    }

    case TOOL_POINTER:
      moveCommit(point);
      break;

    case TOOL_MEASURE:
      clearMeasurement();
      break;

    default:
      break;
  }
  drawing = false;
}

QMenu *MapScene::menuByMiniatureInstance(MiniatureInstance *instance)
{
  MapMiniatureResource resource = instance->getMiniature();
  QMenu* menu = new QMenu(resource.getName());
  if(!resource.getName().isEmpty())
    menu->addAction(resource.getName())->setDisabled(true);
  else
  {
    menu->addAction("Unnamed Miniature")->setDisabled(true);
    menu->setTitle("Unnamed Miniature");
  }
  menu->addSeparator();
  menu->addAction(GlobalGUI::freedesktopTextEditor(),"Edit",this,&MapScene::editMiniatureAction);
  QMenu* visibilityMenu = menu->addMenu("Visibility");
  visibilityMenu->addAction("Visible",this,&MapScene::setVisible);
  visibilityMenu->addAction("Partial",this,&MapScene::setVisiblePartial);
  visibilityMenu->addAction("Invisible",this,&MapScene::setInvisible);
  QMenu* rotateMenu = menu->addMenu("Rotate To");
  rotateMenu->addAction("↑   (0°)",this,&MapScene::rotateMiniatureNorth);
  rotateMenu->addAction("→  (90°)",this,&MapScene::rotateMiniatureEast);
  rotateMenu->addAction("↓ (180°)",this,&MapScene::rotateMiniatureSouth);
  rotateMenu->addAction("← (270°)",this,&MapScene::rotateMiniatureWest);
  QMenu* directionMenu = menu->addMenu("Direction");
  directionMenu->addAction("None",this,&MapScene::directionMiniatureNone);
  directionMenu->addSeparator();
  directionMenu->addAction("↑",this,&MapScene::directionMiniatureN);
  directionMenu->addAction("↗",this,&MapScene::directionMiniatureNE);
  directionMenu->addAction("→",this,&MapScene::directionMiniatureE);
  directionMenu->addAction("↘",this,&MapScene::directionMiniatureSE);
  directionMenu->addAction("↓",this,&MapScene::directionMiniatureS);
  directionMenu->addAction("↙",this,&MapScene::directionMiniatureSW);
  directionMenu->addAction("←",this,&MapScene::directionMiniatureW);
  directionMenu->addAction("↖",this,&MapScene::directionMiniatureNW);
  menu->addSeparator();
  menu->addAction(GlobalGUI::freedesktopCopy(),"Copy",this,&MapScene::copyMiniature);
  menu->addAction(GlobalGUI::freedesktopCut(),"Cut",this,&MapScene::cutMiniature);
  menu->addAction(GlobalGUI::freedesktopDelete(),"Remove",this,&MapScene::removeMiniatureMsg);
  menu->addAction("Export to Minis",this,&MapScene::exportToMinis);
  menu->addSeparator();
  QAction* reloadAction = menu->addAction("Reload Image",this,&MapScene::reloadMiniature);
  reloadAction->setDisabled(QColor(resource.getGraphic()).isValid());
  menu->addSeparator();
  QAction* initiativeAction = menu->addAction("Add To Initiative List",this,&MapScene::addMiniatureToInitiative);
  if(orpg::ClientCore::get()->getRole()<ROLE_GM)
    initiativeAction->setDisabled(true);
  QAction* errorAction = menu->addAction("Show Error",this,&MapScene::displayMiniErrorMessage);
  if(instance->getError().isEmpty())
    errorAction->setDisabled(true);
  miniatureEditMap.insert(menu,resource);
  return menu;
}

QMenu *MapScene::menuByMiniatureInstances(const QList<MiniatureInstance *> &instances)
{
  QList<QMenu*> menus;
  foreach(MiniatureInstance* instance, instances)
  {
    menus.append(menuByMiniatureInstance(instance));
  }
  if(menus.size()==0)
  {
    return nullptr;
  }
  else if(menus.size()==1)
  {
    return menus.first();
  }
  else
  {
    QMenu* mainMenu = menus.first();
    menus.removeAt(0);
    QMenu* otherMenu = new QMenu("Other Miniatures");
    mainMenu->insertMenu(mainMenu->actions()[1],otherMenu);
    foreach(QMenu* subMenu, menus)
      otherMenu->addMenu(subMenu);
    return mainMenu;
  }
}

QMenu *MapScene::getGeneralContextMenu(const QPointF& pos)
{
  QJsonParseError error;
  QJsonDocument doc = QJsonDocument::fromJson(QGuiApplication::clipboard()->text().toUtf8(),&error);
  if(error.error!=QJsonParseError::NoError)
    return nullptr;
  miniaturePasteCandidate = MapMiniatureResource(doc.object());
  if(!miniaturePasteCandidate.isValid())
    return nullptr;
  miniaturePasteCandidate.setPos(getGridPoint(pos,TOOL_MINIATURE,miniaturePasteCandidate)/gridSize);
  QMenu* menu = new QMenu();
  menu->addAction(GlobalGUI::freedesktopPaste(),"Paste",this,&MapScene::pasteMiniature);
  return menu;
}

bool MapScene::fogBlock(const QPointF &point)
{
  return (status<ROLE_GM) && fog->hides(point);
}

void MapScene::setZoom(const qreal &value)
{
  zoom = value;
}

QPointF MapScene::getGridPoint(QPointF point)
{
  return getGridPoint(point,currentTool, miniature);
}

QPointF MapScene::getGridPoint(QPointF point, Tool tool, const MapMiniatureResource& miniature)
{
  switch(tool)
  {
    //align between grid lines
    case TOOL_POINTER:
    {
      //this is handled by the MiniatureItemCollection class
      return point;
    }
    case TOOL_MEASURE:
    case TOOL_MINIATURE:
    {
      if(snapToGrid==0.0)
        return point;
      qreal virtGridSize;
      qreal miniatureSize = miniature.getSize();
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
      point = QPointF(virtGridSize * (std::floor(point.x()/virtGridSize)+gridOffset), virtGridSize * (std::floor(point.y()/virtGridSize)+gridOffset));
      return point;
    }
    //do not align with grid lines
    case TOOL_ERASER:
    case TOOL_FOG:
    case TOOL_CLEARFOG:
    {
      return point;
    }
    //align on grid lines
    default:
    {
      if(snapToGrid==0.0)
        return point;
      qreal gridSize = this->gridSize/snapToGrid;
      QPointF newPoint(gridSize * std::round(point.x()/gridSize), gridSize * std::round(point.y()/gridSize));
      return newPoint;
    }
  }
}

void MapScene::dividePointVector(QVector<QPointF> &vector)
{
  QPointF newPoint;
  for(int i=0;i<vector.size()-1;i++)
  {
    if(euclidDistance(vector[i],vector[i+1]) > distanceThreshold)
    {
      newPoint = (vector[i]+vector[i+1])/2.0;
      vector.insert(i+1,newPoint);
      i--;
    }
  }
  for(int i=0;i<vector.size();i++)
  {
    vector[i] /= gridSize;
  }
}

void MapScene::clearToolPreview()
{
  if(toolPreview)
  {
    this->removeItem(toolPreview);
    delete toolPreview;
    toolPreview = NULL;
  }
}

QPen MapScene::getPen()
{
  return QPen(brush,brushSize,Qt::SolidLine,Qt::RoundCap);
}

int MapScene::getEraserSize()
{
  return std::max(1,brushSize*4);
}

void MapScene::drawMiniaturePreview(qreal x, qreal y)
{
  clearMiniaturePreview();
  miniaturePreview = ih.getGraphicsItem(miniature.getGraphic(),miniature.getSize()*gridSize,miniature.getDisplay());
  miniaturePreview->setPos(x,y);
  addItem(miniaturePreview);
}

void MapScene::clearMiniaturePreview()
{
  if(miniaturePreview)
  {
    ih.removeGraphicsItem(miniaturePreview);
    removeItem(miniaturePreview);
    delete miniaturePreview;
    miniaturePreview = NULL;
  }
}

bool MapScene::initMiniatureEditMiniature(QObject *action)
{
  if(miniatureEditMap.contains(action->parent()))
  {
    miniatureEditMiniature = miniatureEditMap[action->parent()];
    return true;
  }
  else
  {
    return false;
  }
}

QList<MiniatureInstance *> MapScene::getMiniatureItems(QPointF point)
{
  QList<MiniatureInstance *> list = minis->getItemsByList(items(point),nullptr,orpg::ClientCore::get()->getVariables()->getMapSettingsContainer()->getVariable()->getLockBackground(),fogBlock(point));
  return list;
}

QGraphicsItem *MapScene::getMiniatureItem(QPointF point)
{
  MiniatureInstance* item = minis->getItemByList(items(point),nullptr,orpg::ClientCore::get()->getVariables()->getMapSettingsContainer()->getVariable()->getLockBackground());
  if(item==nullptr)
  {
    return nullptr;
  }
  if( (item->getMiniature().getLayer() != MapMiniatureResource::LAYER_ABOVEFOG) && fogBlock(point))
  {
    return nullptr;
  }
  else
    return item->getItem();
}

void MapScene::resetMiniaturePos()
{
  minis->undoGrab();
}

void MapScene::moveMiniature(QPointF point)
{
  minis->dragItem(point);
}

void MapScene::grabMiniature(QPointF point)
{
  minis->grabItem(point,items(point),orpg::ClientCore::get()->getVariables()->getMapSettingsContainer()->getVariable()->getLockBackground(),fogBlock(point));
}

void MapScene::moveCommit(QPointF point)
{
  MapMiniatureResource mini;
  if(minis->commitDrag(point,mini))
  {
    editMiniature(mini);
  }
}

QGraphicsTextItem *MapScene::getTextItem(QPointF point)
{
  if(fogBlock(point))
    return nullptr;
  QGraphicsTextItem* tItem = NULL;
  QList<QGraphicsItem*> itemsAtPos = items(point);
  foreach(QGraphicsItem* item, itemsAtPos)
  {
    if(mapTextMap.values().contains((QGraphicsTextItem*)item))
    {
      tItem = (QGraphicsTextItem*)item;
      break;
    }
  }
  return tItem;
}

void MapScene::clearMapText()
{
  foreach(QGraphicsTextItem* item, mapTextMap.values())
  {
    delete item;
  }
  mapTextMap.clear();
}

void MapScene::clearMeasurement()
{
  delete measureLine;
  delete measureCircle;
  delete measureText;
  measureLine = nullptr;
  measureCircle = nullptr;
  measureText = nullptr;
}

QColor MapScene::getVisibleColor()
{
  QColor col = backgroundBrush().color();
  qreal distance = col.red()+col.green()+col.blue();
  if(distance/3<128)
    return Qt::white;
  else
    return Qt::black;
}

void MapScene::drawMeasurement(QPointF point)
{
  clearMeasurement();
  QColor col = getVisibleColor();
  measureLine = addLine(QLine(measureStart.toPoint(),point.toPoint()),QPen(QBrush(col),2,Qt::DashLine));
  double ed = std::hypot((double)measureStart.x()-point.x(),(double)measureStart.y()-point.y());
  measureCircle = addEllipse(measureStart.x()-ed,measureStart.y()-ed,2*ed,2*ed,QPen(QBrush(col),1,Qt::DashLine));
  QFont font;
  font.setPixelSize(20/(zoom));
  measureText = addText(orpg::Settings::get()->distanceMeasure.distanceString(point/gridSize,measureStart/gridSize),font);
  measureText->setDefaultTextColor(col);
  measureText->setPos(point.x(),point.y()+measureText->boundingRect().height());
}

void MapScene::updateViews()
{
  foreach(QGraphicsView* view, views())
    view->update();
}

void MapScene::updateAdditionalInfo(const QPointF &pos)
{
  QList<QGraphicsItem*> itemsAtPoint = items(pos);
  bool aboveFog = minis->showAdditionalInfo(itemsAtPoint,zoom);
  if(!aboveFog && fogBlock(pos))
  {
    minis->hideCurrentAdditionalInfo();
  }
}

void MapScene::newMiniature(const MapMiniatureResource &miniature)
{
  sendMessage(orpg::RoomResourceMessage::newResourceMessage(miniature));
}

void MapScene::editMiniature(const MapMiniatureResource &miniature)
{
  sendMessage(orpg::RoomResourceMessage::setResourceMessage(miniature));
}

void MapScene::sendMessage(const orpg::Message &msg)
{
  orpg::ClientCore::get()->send(msg);
}

void MapScene::sendMessage(const QList<orpg::Message> &msgs)
{
  foreach(const orpg::Message& msg, msgs)
    sendMessage(msg);
}

void MapScene::setUpdates(bool value)
{
  foreach(QGraphicsView* view, views())
  {
    view->setUpdatesEnabled(value);
  }
}

MapScene::MapScene(QObject *parent) : QGraphicsScene(parent)
{
  backgroundColor = Qt::white;
  nightMode = false;
  updateBackground();
  zoom = 1.0;
  brushColor = Qt::black;
  brush = QBrush(brushColor);
  selectedTool = TOOL_PENCIL;
  drawing = false;
  snapToGrid = 4.0;
  toolPreview = NULL;
  gridSize = 25.0;
  distanceThreshold = gridSize/4;

  minis = new MiniatureInstanceCollection(&ih,gridSize,snapToGrid,this);
  fog = new FogInstanceCollection(gridSize,this);
  fog->setFogColor(QColor(80,80,80,100));
  gridGroup = new QGraphicsItemGroup;
  lineGroup = new QGraphicsItemGroup;

  this->addItem(minis->getGroupBackground());
  this->addItem(gridGroup);
  this->addItem(minis->getGroupLow());
  this->addItem(lineGroup);
  this->addItem(minis->getGroupHigh());
  this->addItem(fog->getItemGroup());
  this->addItem(minis->getGroupHighest());
  this->addItem(minis->getGroupNames());

  miniaturePreview = NULL;
  miniature = MapMiniatureResource();
  measureLine = nullptr;
  measureCircle = nullptr;
  measureText = nullptr;

  //lines
  connect(orpg::ClientCore::get()->getResources()->getMapLineContainer(),&MapLineResourceContainer::resourceChanged,this,&MapScene::setLine);
  connect(orpg::ClientCore::get()->getResources()->getMapLineContainer(),&MapLineResourceContainer::resourceRemoved,this,&MapScene::removeLine);

  //fog
  connect(orpg::ClientCore::get()->getResources()->getMapFogContainer(),&MapFogResourceContainer::resourceChanged,this,&MapScene::setFog);
  connect(orpg::ClientCore::get()->getResources()->getMapFogContainer(),&MapFogResourceContainer::resourceRemoved,this,&MapScene::removeFog);
  connect(fog,&FogInstanceCollection::fogGenerated,this,&MapScene::generatedFog);
  connect(fog,&FogInstanceCollection::fogRemoved,this,&MapScene::removedFog);

  //text
  connect(orpg::ClientCore::get()->getResources()->getMapTextContainer(),&MapTextResourceContainer::resourceChanged,this,&MapScene::setText);
  connect(orpg::ClientCore::get()->getResources()->getMapTextContainer(),&MapTextResourceContainer::resourceRemoved,this,&MapScene::removeText);

  //minis
  connect(orpg::ClientCore::get()->getResources()->getMapMiniatureContainer(),&ResourceContainer::resourceChanged,this,&MapScene::setMiniature);
  connect(orpg::ClientCore::get()->getResources()->getMapMiniatureContainer(),&ResourceContainer::resourceRemoved,this,&MapScene::removeMiniature);

  connect(orpg::ClientCore::get()->getVariables()->getMapSettingsContainer(),&MapSettingsVariableContainer::variableChanged,this,&MapScene::setMapSettings);
}

void MapScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
  mouseEvent->accept();
}

void MapScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
  if(drawing && mouseEvent->buttons() == Qt::LeftButton)
  {
    drawPoint(getGridPoint(mouseEvent->scenePos()));
  }
  if(selectedTool == TOOL_ERASER || selectedTool == TOOL_FOG || selectedTool == TOOL_CLEARFOG)
  {
    clearToolPreview();
    if((mouseEvent->buttons() == Qt::NoButton || mouseEvent->buttons() == Qt::LeftButton))
    {
      toolPreview = new QGraphicsEllipseItem(mouseEvent->scenePos().x()-getEraserSize(),mouseEvent->scenePos().y()-getEraserSize(),2*getEraserSize(),2*getEraserSize());
      ((QGraphicsEllipseItem*)toolPreview)->setPen(QPen(getVisibleColor()));
      this->addItem(toolPreview);
    }
  }
  updateAdditionalInfo(mouseEvent->scenePos());
}

void MapScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
  if(mouseEvent->buttons() == Qt::LeftButton)
  {
    currentTool = selectedTool;
    drawing = true;
    drawInit(getGridPoint(mouseEvent->scenePos()));
  }
  else
  {
    if(drawing)
      cancelDrawing();
  }
}

void MapScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
  if(drawing && mouseEvent->button() == Qt::LeftButton)
  {
    drawCommit(getGridPoint(mouseEvent->scenePos()));
  }
  else if((selectedTool == TOOL_POINTER || selectedTool == TOOL_MINIATURE || selectedTool == TOOL_TEXT) && mouseEvent->button() == Qt::RightButton)
  {
    QGraphicsTextItem* textItem;
    miniatureEditMap.clear();
    QMenu* menu; ;
    if( ( menu = menuByMiniatureInstances(getMiniatureItems(mouseEvent->scenePos())) ) )
    {
      menu->exec(mouseEvent->screenPos());
      delete menu;
    }
    else if( (textItem = getTextItem(mouseEvent->scenePos()) ) )
    {
      currentMapTextItemId = textItem->data(0).toInt();
      QMenu menu(static_cast<QWidget*>(parent()));
      menu.addAction("Text");
      menu.actions().at(0)->setDisabled(true);
      menu.addAction(GlobalGUI::freedesktopTextEditor(),"Edit",this,&MapScene::editMapTextSlot);
      menu.addAction(GlobalGUI::freedesktopDelete(),"Remove",this,&MapScene::removeMapTextSlot);
      menu.exec(mouseEvent->screenPos());
    }
    else if( (menu = getGeneralContextMenu(mouseEvent->scenePos()) ) )
    {
      menu->exec(mouseEvent->screenPos());
      delete menu;
    }
  }
  cancelDrawing();
}

void MapScene::setTool(Tool tool)
{
  if(drawing)
    cancelDrawing();
  clearToolPreview();
  selectedTool = tool;
}

void MapScene::setStatus(PlayerRole value)
{
  this->status = value;
  fog->setPlayerRole(value);
  minis->setShowInvisible(value>=ROLE_GM);
}

void MapScene::setBrushSize(int brushSize)
{
  this->brushSize = brushSize;
}

void MapScene::setSnapping(qreal value)
{
  snapToGrid = value;
  minis->setSnapToGrid(value);
}

void MapScene::setBrushColor(QColor color)
{
  this->brushColor = color;
  brush.setColor(color);
}

void MapScene::setMiniaturePreview(const MapMiniatureResource& miniature)
{
  this->miniature = miniature;
}

void MapScene::exportToMinis()
{
  if(!initMiniatureEditMiniature(sender()))
    return;
  emit miniatureExported(miniatureEditMiniature);
}

void MapScene::editMiniatureAction()
{
  if(!initMiniatureEditMiniature(sender()))
    return;
  MiniatureEditDialog dialog;
  dialog.setMiniature(miniatureEditMiniature);
  if(dialog.exec())
  {
    editMiniature(dialog.getMiniature());
  }
}

void MapScene::setMiniRotation(qreal deg)
{
  if(!initMiniatureEditMiniature(sender()->parent()))
    return;
  miniatureEditMiniature.setRotation(deg);
  editMiniature(miniatureEditMiniature);
}

void MapScene::rotateMiniatureNorth()
{
  setMiniRotation(0);
}

void MapScene::rotateMiniatureSouth()
{
  setMiniRotation(180);
}

void MapScene::rotateMiniatureWest()
{
  setMiniRotation(270);
}

void MapScene::rotateMiniatureEast()
{
  setMiniRotation(90);
}

void MapScene::setMiniDirection(qreal deg)
{
  if(!initMiniatureEditMiniature(sender()->parent()))
    return;
  miniatureEditMiniature.setDirection(deg);
  editMiniature(miniatureEditMiniature);
}

void MapScene::directionMiniatureNone()
{
  setMiniDirection(-1);
}

void MapScene::directionMiniatureN()
{
  setMiniDirection(0);
}

void MapScene::directionMiniatureNE()
{
  setMiniDirection(45);
}

void MapScene::directionMiniatureE()
{
  setMiniDirection(90);
}

void MapScene::directionMiniatureSE()
{
  setMiniDirection(135);
}

void MapScene::directionMiniatureS()
{
  setMiniDirection(180);
}

void MapScene::directionMiniatureSW()
{
  setMiniDirection(225);
}

void MapScene::directionMiniatureW()
{
  setMiniDirection(270);
}

void MapScene::directionMiniatureNW()
{
  setMiniDirection(315);
}

void MapScene::setVisibility(MapMiniatureResource::Visibility visibility)
{
  if(!initMiniatureEditMiniature(sender()->parent()))
    return;
  miniatureEditMiniature.setVisibility(visibility);
  editMiniature(miniatureEditMiniature);
}

void MapScene::setVisible()
{
  setVisibility(MapMiniatureResource::VISIBILITY_VISIBLE);
}

void MapScene::setVisiblePartial()
{
  setVisibility(MapMiniatureResource::VISIBILITY_PARTIAL);
}

void MapScene::setInvisible()
{
  setVisibility(MapMiniatureResource::VISIBILITY_INVISIBLE);
}

void MapScene::addMiniatureToInitiative()
{
  if(!initMiniatureEditMiniature(sender()))
    return;
  InitiativeResource res(miniatureEditMiniature.getName(),QString(),miniatureEditMiniature.getGraphic(),false,0,0);
  orpg::ClientCore::get()->newInitiativeResource(res);
}

void MapScene::copyMiniature()
{
  if(!initMiniatureEditMiniature(sender()))
    return;
  QGuiApplication::clipboard()->setText(QString(QJsonDocument(miniatureEditMiniature.data()).toJson()));
}

void MapScene::cutMiniature()
{
  if(!initMiniatureEditMiniature(sender()))
    return;
  QGuiApplication::clipboard()->setText(QString(QJsonDocument(miniatureEditMiniature.data()).toJson()));
  sendMessage(orpg::RoomResourceMessage::removeResourceMessage(miniatureEditMiniature));
}

void MapScene::pasteMiniature()
{
  if(miniaturePasteCandidate.isValid())
    newMiniature(miniaturePasteCandidate);
}

void MapScene::removeMiniatureMsg()
{
  if(!initMiniatureEditMiniature(sender()))
    return;
  sendMessage(orpg::RoomResourceMessage::removeResourceMessage(miniatureEditMiniature));
}

void MapScene::reloadMiniature()
{
  if(!initMiniatureEditMiniature(sender()))
    return;
  QMessageBox msgBox;
  msgBox.setText("Reload miniature image?");
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::Yes);
  if(msgBox.exec()==QMessageBox::Yes)
  {
    ih.reload(miniatureEditMiniature.getGraphic());
  }
}

void MapScene::displayMiniErrorMessage()
{
  if(!initMiniatureEditMiniature(sender()))
    return;
  MiniatureInstance* miniItem = minis->getItemByMiniature(miniatureEditMiniature);
  if(miniItem!=nullptr)
    QMessageBox::information((QWidget*)this->parent(),"Miniature Image Error",miniItem->getError(),QMessageBox::Ok);
}

void MapScene::editMapTextSlot()
{
  if(mapTextMap.contains(currentMapTextItemId))
  {
    MapTextResource* textPtr = orpg::ClientCore::get()->getResources()->getMapTextContainer()->getTextResource(currentMapTextItemId);
    if(textPtr==nullptr)
      return;
    MapTextDialog dialog;
    dialog.setText(textPtr->getText());
    dialog.setSize(textPtr->getSize());
    if(dialog.exec())
    {
      MapTextResource* textPtr = orpg::ClientCore::get()->getResources()->getMapTextContainer()->getTextResource(currentMapTextItemId);
      if(textPtr==nullptr)
        return;
      MapTextResource text = *textPtr;
      text.setSize(dialog.getSize());
      text.setText(dialog.getText());
      sendMessage(orpg::RoomResourceMessage::setResourceMessage(text));
    }
  }
}

void MapScene::removeMapTextSlot()
{
  if(mapTextMap.contains(currentMapTextItemId))
  {
    sendMessage(orpg::RoomResourceMessage::removeResourceMessage(MapTextResource::resourceName,currentMapTextItemId));
  }
}

void MapScene::setLine(QList<int> ids)
{
  removeLine(ids);
  foreach(int id, ids)
  {
    QGraphicsLineItem* item;
    MapLineResource* res = orpg::ClientCore::get()->getResources()->getMapLineContainer()->getLineResource(id);
    QBrush b(QColor(res->getColor()));
    QLineF line = res->getLine();
    line.setP1(line.p1()*gridSize);
    line.setP2(line.p2()*gridSize);
    item = this->addLine(line,QPen(b,res->getWidth(),Qt::SolidLine,Qt::RoundCap));
    lineGroup->addToGroup(item);
    item->setData(0,id);
    lineMap.insert(id,item);
    lineItemSet.insert(item);
  }
}

void MapScene::removeLine(QList<int> ids)
{
  foreach(int id, ids)
  {
    QGraphicsItem* item = lineMap[id];
    if(item!=nullptr)
    {
      lineItemSetEdit.remove(item);
      lineItemSet.remove(item);
      delete item;
      lineMap.remove(id);
    }
  }
}


void MapScene::setFog(QList<int> ids)
{
  foreach(int id, ids)
  {
    MapFogResource* resource = orpg::ClientCore::get()->getResources()->getMapFogContainer()->getFogResource(id);
    if(resource!=nullptr)
      fog->addFog(*resource);
  }
}

void MapScene::removeFog(QList<int> ids)
{
  foreach(int id, ids)
  {
    fog->removeFog(id);
  }
}

void MapScene::setText(QList<int> ids)
{
  removeText(ids);
  foreach(int id, ids)
  {
    MapTextResource* textPtr = orpg::ClientCore::get()->getResources()->getMapTextContainer()->getTextResource(id);
    if(textPtr==nullptr)
      return;
    QFont font;
    font.setPixelSize(textPtr->getSize());
    QGraphicsTextItem* item = new QGraphicsTextItem(textPtr->getText());
    item->setFont(font);
    item->setDefaultTextColor(QColor(textPtr->getColor()));
    item->setPos(textPtr->getPos().x()-item->boundingRect().width()/2,textPtr->getPos().y()-item->boundingRect().height()/2);
    item->setGroup(lineGroup);
    item->setData(0,id);
    mapTextMap.insert(id,item);
  }
}

void MapScene::removeText(QList<int> ids)
{
  foreach(int id, ids)
  {
    QGraphicsTextItem* item = mapTextMap[id];
    if(item!=nullptr)
    {
      delete item;
      mapTextMap.remove(id);
    }
  }
}

void MapScene::setMiniature(QList<int> ids)
{
  foreach(int id, ids)
  {
    minis->addMini(*orpg::ClientCore::get()->getResources()->getMapMiniatureContainer()->getMiniatureResource(id));
  }
}

void MapScene::removeMiniature(QList<int> ids)
{
  foreach(int id, ids)
  {
    minis->removeMini(id);
  }
}

void MapScene::generatedFog(QList<MapFogResource> resources)
{
  QList<GenericResource> gresources;
  foreach(const MapFogResource& resource, resources)
  {
    gresources.append(resource);
  }
  sendMessage(orpg::RoomResourceMessage::newResourceMessage(gresources));
}

void MapScene::removedFog(QList<int> indices)
{
  sendMessage(orpg::RoomResourceMessage::removeResourceMessage(MapFogResource::resourceName,indices));
}

void MapScene::setMapSettings()
{
  MapSettingsVariable settings = *(orpg::ClientCore::get()->getVariables()->getMapSettingsContainer()->getVariable());
  backgroundColor = settings.getBgColor();
  updateBackground();
  ih.setFrameColor(getVisibleColor());
  drawGrid(settings.getSizeX(),settings.getSizeY(),settings.getHideGrid());
  colorGrid(QColor(settings.getGridColor()));
  minis->setOutlineColor(getVisibleColor());
}
