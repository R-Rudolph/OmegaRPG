#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <cmath>
#include <QObject>
#include <QMap>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItemGroup>
#include <QPointF>

#include <QPair>
#include <QGraphicsLineItem>
#include <QMenu>
#include <QTimer>
#include "core/multi/message/message.h"
#include "gui/client/objects/externalpixmapitemhandler.h"
#include "core/multi/util/global.h"
#include "gui/util/globalgui.h"
#include "gui/map/dialogs/miniatureeditdialog.h"
#include "dialogs/maptextdialog.h"

#include <objects/mapscene-instances/miniatureinstancecollection.h>
#include <objects/mapscene-instances/foginstancecollection.h>
#include <core/client/util/distancemeasure.h>


enum Tool
{
  TOOL_POINTER,
  TOOL_PENCIL,
  TOOL_LINE,
  TOOL_RECTANGLE,
  TOOL_ERASER,
  TOOL_TEXT,
  TOOL_FOG,
  TOOL_CLEARFOG,
  TOOL_NONE,
  TOOL_MINIATURE,
  TOOL_MEASURE
};

class MapScene : public QGraphicsScene
{
  Q_OBJECT
  //message generation
  orpg::Message lineMessagesFromVector(const QVector<QPointF>& points, int width, const QString& color);
  //static stuff
  ExternalPixmapItemHandler ih;
  MiniatureInstanceCollection* minis;
  FogInstanceCollection* fog;

  qreal distanceThreshold;
  qreal gridSize;

  //settings
  QColor backgroundColor;
  bool nightMode;
  void updateBackground();
  QBrush brush;
  int brushSize;
  QColor brushColor;
  bool drawing;
  Tool selectedTool;
  Tool currentTool;
  PlayerRole status;
  qreal x1,x2,y1,y2;
  qreal snapToGrid;


  //item groups
  QGraphicsItemGroup* gridGroup;
  QGraphicsItemGroup* lineGroup;



  QGraphicsItem* toolPreview;
  //line fields
  QMap<int,QGraphicsItem*> lineMap;
  QSet<QGraphicsItem*> lineItemSet;
  QSet<QGraphicsItem*> lineItemSetEdit;
  QVector<QGraphicsItem*> previewItems;
  //grid fields
  QSet<QGraphicsItem*> gridItems;
  //Miniature fields


  QVector<QPointF> pointVec;
  QPointF lastPoint;

  void setCanvasSize(qreal x1, qreal x2, qreal y1, qreal y2);
  void drawGrid();
  void drawGrid(int x, int y, bool hideGrid);
  void colorGrid(const QColor& color);
  void cancelDrawing();
  void drawPoint(QPointF point);
  void drawInit(QPointF point);
  void drawCommit(QPointF point);
  QMenu* menuByMiniatureInstance(MiniatureInstance* instance);
  QMenu* menuByMiniatureInstances(const QList<MiniatureInstance*>& instances);
  QMenu* getGeneralContextMenu(const QPointF& pos);

  bool fogBlock(const QPointF& point);

  qreal zoom;

  QPointF getGridPoint(QPointF point);
  QPointF getGridPoint(QPointF point, Tool tool, const MapMiniatureResource& miniature);
  void dividePointVector(QVector<QPointF>& vector);
  void clearToolPreview();
  QPen getPen();
  int getEraserSize();

  //miniature preview stuff
  QGraphicsItem* miniaturePreview;
  MapMiniatureResource miniature;
  void drawMiniaturePreview(qreal x, qreal y);
  void clearMiniaturePreview();

  //miniature edit stuff
  QMap<QObject*,MapMiniatureResource> miniatureEditMap;
  bool initMiniatureEditMiniature(QObject* action);
  MapMiniatureResource miniatureEditMiniature;
  MapMiniatureResource miniaturePasteCandidate;

  //miniature move stuff
  QList<MiniatureInstance*> getMiniatureItems(QPointF point);
  QGraphicsItem* getMiniatureItem(QPointF point);
  QPointF miniatureMoveOffset;
  void resetMiniaturePos();
  void moveMiniature(QPointF point);
  void grabMiniature(QPointF point);
  void moveCommit(QPointF point);

  //map text functions
  QMap<int,QGraphicsTextItem*> mapTextMap;
  int currentMapTextItemId;
  QGraphicsTextItem* getTextItem(QPointF point);
  void clearMapText();

  //measure tool functionn
  QGraphicsLineItem* measureLine;
  QGraphicsEllipseItem* measureCircle;
  QGraphicsTextItem* measureText;
  QPointF measureStart;
  void clearMeasurement();
  QColor getVisibleColor();
  void drawMeasurement(QPointF point);

  void updateViews();
  void updateAdditionalInfo(const QPointF& pos);

  void newMiniature(const MapMiniatureResource& miniature);
  void editMiniature(const MapMiniatureResource& miniature);
  void sendMessage(const orpg::Message& msg);
  void sendMessage(const QList<orpg::Message>& msgs);

  void setUpdates(bool value);
public:
  explicit MapScene(QObject *parent = 0);
  virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
  virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);

  void setZoom(const qreal &value);
signals:
  void mapMessageGenerated(orpg::Message);
  void miniatureExported(const MapMiniatureResource& mini);
  void requestRemoveMiniature(int id);
public slots:
  void setTool(Tool tool);
  void setStatus(PlayerRole value);
  void setBrushSize(int brushSize);
  void setSnapping(qreal value);
public slots:
  void setBrushColor(QColor color);
  void setMiniaturePreview(const MapMiniatureResource& miniature);
private slots:
  void exportToMinis();
  void editMiniatureAction();
  void setMiniRotation(qreal deg);
  void rotateMiniatureNorth();
  void rotateMiniatureSouth();
  void rotateMiniatureWest();
  void rotateMiniatureEast();
  void setMiniDirection(qreal deg);
  void directionMiniatureNone();
  void directionMiniatureN();
  void directionMiniatureNE();
  void directionMiniatureE();
  void directionMiniatureSE();
  void directionMiniatureS();
  void directionMiniatureSW();
  void directionMiniatureW();
  void directionMiniatureNW();
  void setVisibility(MapMiniatureResource::Visibility visibility);
  void setVisible();
  void setVisiblePartial();
  void setInvisible();
  void addMiniatureToInitiative();
  void copyMiniature();
  void cutMiniature();
  void pasteMiniature();
  void removeMiniatureMsg();
  void reloadMiniature();
  void displayMiniErrorMessage();
  void editMapTextSlot();
  void removeMapTextSlot();

  //resource-based slots (Resource -> Map)
  void setLine(QList<int> ids);
  void removeLine(QList<int> ids);
  void setFog(QList<int> ids);
  void removeFog(QList<int> ids);
  void setText(QList<int> ids);
  void removeText(QList<int> ids);
  void setMiniature(QList<int> ids);
  void removeMiniature(QList<int> ids);
  //resource-based slots (Map -> Resource)
  void generatedFog(QList<MapFogResource> resources);
  void removedFog(QList<int> indices);

  //room variable-based slots
  void setMapSettings();
};

#endif // MAPSCENE_H
