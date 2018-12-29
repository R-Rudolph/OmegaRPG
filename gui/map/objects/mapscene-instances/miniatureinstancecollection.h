#ifndef MINIATUREITEMCOLLECTION_H
#define MINIATUREITEMCOLLECTION_H

#include <QGraphicsItem>
#include <QList>
#include <QGraphicsPolygonItem>

#include "core/multi/roomresources/mapresources/mapminiatureresource.h"
#include <gui/client/objects/externalpixmapitemhandler.h>
#include "objects/mapscene-instances/miniatureinstance.h"

class MiniatureInstanceCollection: public QObject
{
  Q_OBJECT
  qreal gridSize;
  qreal snapToGrid;
  ExternalPixmapItemHandler* ih;
  QMap<MapMiniatureResource::Layer,QGraphicsItemGroup*> layerGroupMap;
  QGraphicsItemGroup* groupBackground;
  QGraphicsItemGroup* groupLow;
  QGraphicsItemGroup* groupHigh;
  QGraphicsItemGroup* groupHighest;
  QGraphicsItemGroup* groupNames;
  QColor visibleColor;
  bool showInvisible;
  QMap<int,MiniatureInstance> minis;

  //drag item fields
  bool grabbedDirectionMarker;
  QGraphicsLineItem* directionLineItem;
  QPointF grabOffset;
  QPointF grabOriginalPos;
  bool grabbing;
  int grabbedItem;
  //previous line visisble fields
  int prevLineInstance;
  bool prevLineSet;
  //item garbage :)
  QList<QGraphicsItem*> garbage;
  QTimer* garbageTimer;
  void addToGarbage(QList<QGraphicsItem*> garbage);
public:
  QPointF getCorrectedPos(QPointF pos);
  explicit MiniatureInstanceCollection(ExternalPixmapItemHandler* ih, qreal gridSize, qreal snapToGrid, QObject* parent=nullptr);
  QGraphicsItem* addMini(const MapMiniatureResource& mini);
  MiniatureInstance* getItemByID(int id);
  MiniatureInstance* getItemByMiniature(const MapMiniatureResource& mini);
  MiniatureInstance* getItemByGraphicsItem(QGraphicsItem* item);
  MiniatureInstance* getItemByList(const QList<QGraphicsItem*>& items, bool* grabbedDirectionMarker=nullptr, bool noBackground=false, bool fogBlocking=false);
  QList<MiniatureInstance*> getItemsByList(const QList<QGraphicsItem*>& items, bool* grabbedDirectionMarker=nullptr, bool noBackground=false, bool fogBlocking=false);
  void removeMini(int id, bool getsReplaced=false);
  void clear();
  QGraphicsItemGroup *getGroupBackground() const;
  QGraphicsItemGroup *getGroupLow() const;
  QGraphicsItemGroup *getGroupHigh() const;
  QGraphicsItemGroup *getGroupHighest() const;
  QGraphicsItemGroup *getGroupNames() const;
  void setOutlineColor(const QColor& color);

  void grabItem(const QPointF& pos, const QList<QGraphicsItem*>& items, bool noBackground=false, bool fogBlocking=false);
  void dragItem(const QPointF& pos);
  void undoGrab();
  bool commitDrag(const QPointF& pos, MapMiniatureResource& result);
  void setSnapToGrid(const qreal &value);
  bool showAdditionalInfo(const QList<QGraphicsItem*>& items, float zoom);
  void hideCurrentAdditionalInfo();
  bool getShowInvisible() const;
  void setShowInvisible(bool value);

private slots:
  void clearGarbage();
};

#endif // MINIATUREITEMCOLLECTION_H
