#ifndef MAPVIEW_H
#define MAPVIEW_H


#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QGraphicsSceneMouseEvent>
#include "objects/mapscene.h"

class MapView : public QGraphicsView
{
  Q_OBJECT
  static const qreal windowHookDistanceThreshold;
  qreal zoomFactor = 1.0;
  QPointF sceneHook;
  qreal windowHookDistance;
  QPoint windowHook, windowHookLast;
  bool pressingMiddle = false;

  void moveSceneToScreenPos(QPointF scenePos, QPointF screenPos);
  void resetZoom();
  void updateSceneZoom();
public:
  explicit MapView(QGraphicsScene* scene = 0, QWidget *parent = 0);

protected:
  virtual void wheelEvent(QWheelEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void mouseDoubleClickEvent(QMouseEvent *event);

signals:

public slots:
};

#endif // MAPVIEW_H
