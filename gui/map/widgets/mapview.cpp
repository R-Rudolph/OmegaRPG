#include "mapview.h"
#include <QOpenGLWidget>

const qreal MapView::windowHookDistanceThreshold = 20;

void MapView::moveSceneToScreenPos(QPointF scenePos, QPointF screenPos)
{
  QPointF p1 = mapToScene(0,0);
  QPointF p2 = mapToScene(this->viewport()->width(),this->viewport()->height());
  qreal factor = ( p2.x() - p1.x() + p2.y() - p1.y() )/(this->viewport()->width() + this->viewport()->height()+2);
  QRectF visibleArea(scenePos.x() - factor*screenPos.x(),
                     scenePos.y() - factor*screenPos.y(),
                     factor*this->viewport()->width(),
                     factor*this->viewport()->height()
                     );
  this->ensureVisible(visibleArea,0,0);
  this->ensureVisible(visibleArea,0,0);
  this->update();
  this->viewport()->update();
}

void MapView::resetZoom()
{
  scale(1.0/zoomFactor,1.0/zoomFactor);
  zoomFactor = 1.0;
  updateSceneZoom();
}

void MapView::updateSceneZoom()
{
  ((MapScene*)this->scene())->setZoom(zoomFactor);
}

MapView::MapView(QGraphicsScene *scene, QWidget *parent) : QGraphicsView(scene, parent)
{
  setCacheMode(QGraphicsView::CacheNone);
  this->setMouseTracking(true);
}

void MapView::wheelEvent ( QWheelEvent * event )
{
  qreal zoom = 1.0 + 0.0008*event->delta();
  zoomFactor *= zoom;
  updateSceneZoom();
  QPointF point = mapToScene(event->pos());
  scale(zoom,zoom);

  moveSceneToScreenPos(point,event->pos());
}

void MapView::mouseMoveEvent(QMouseEvent *event)
{
  QGraphicsView::mouseMoveEvent(event);
  if(event->buttons() == Qt::RightButton || event->buttons() == Qt::MiddleButton)
  {
    QPoint diff = event->pos() - windowHook;
    windowHookDistance += qSqrt(diff.x()*diff.x()+diff.y()*diff.y());
    windowHook = event->pos();
    moveSceneToScreenPos(sceneHook,event->pos());
  }
  else if(event->buttons() == (Qt::LeftButton | Qt::RightButton))
  {
    windowHookDistance = windowHookDistanceThreshold;
    int diff = windowHookLast.y() - event->pos().y();
    windowHookLast = event->pos();
    qreal zoom = 1.0 + 0.01*diff;
    zoomFactor *= zoom;
    updateSceneZoom();

    scale(zoom,zoom);
    moveSceneToScreenPos(sceneHook,windowHook);
  }
}

void MapView::mousePressEvent(QMouseEvent *event)
{
  QGraphicsView::mousePressEvent(event);
  if(event->buttons() == Qt::RightButton || event->buttons() == Qt::MiddleButton)
  {
    windowHookDistance = 0;
    windowHook = event->pos();
    sceneHook = mapToScene(event->pos());
  }
  else if(event->buttons() == (Qt::LeftButton | Qt::RightButton))
  { //prepare 2-button-zoom
    sceneHook = mapToScene(event->pos());
    windowHook = event->pos();
    windowHookLast = event->pos();
  }
}

void MapView::mouseReleaseEvent(QMouseEvent *event)
{
  if(event->button()!=Qt::RightButton)
    QGraphicsView::mouseReleaseEvent(event);
  else if(windowHookDistance<windowHookDistanceThreshold)
  {
    QGraphicsView::mouseReleaseEvent(event);
  }
}

void MapView::mouseDoubleClickEvent(QMouseEvent *event)
{
  if(event->button() == Qt::MiddleButton || event->button() == Qt::RightButton)
  {
    resetZoom();
  }
}

