#include "graphicscircledeffect.h"

QColor GraphicsCircledEffect::getFrameColor() const
{
  return frameColor;
}

void GraphicsCircledEffect::setFrameColor(const QColor &value)
{
  frameColor = value;
}

GraphicsCircledEffect::GraphicsCircledEffect(QObject *parent) : QGraphicsEffect(parent)
{
  frameColor = Qt::black;
}

GraphicsCircledEffect::GraphicsCircledEffect(const QColor &color, QObject *parent) : GraphicsCircledEffect(parent)
{
  frameColor = color;
}

QRectF GraphicsCircledEffect::boundingRectFor(const QRectF &rect) const
{
  QPointF center = rect.center();
  qreal maxExp = qMax(rect.height(),rect.width());
  return QRectF(center.x()-maxExp/2,center.y()-maxExp/2,maxExp,maxExp);
}

void GraphicsCircledEffect::draw(QPainter *painter)
{
  QPoint offset;
  if (sourceIsPixmap())
  {
    // No point in drawing in device coordinates (pixmap will be scaled anyways).
    QPixmap pixmap = sourcePixmap(Qt::LogicalCoordinates, &offset);
    QSizeF size = sourceBoundingRect().size();
    QPoint center = offset+QPoint(pixmap.size().width()/2,pixmap.size().height()/2);

    int maxExpansion = qMax(size.width(),size.height());
    qreal sizeFactor = (maxExpansion-maxExpansion/32)/qSqrt(size.height()*size.height()+size.width()*size.width());

    QRectF target = QRectF(center.x()-sizeFactor*pixmap.size().width()/2,
                           center.y()-sizeFactor*pixmap.size().height()/2,
                           sizeFactor*pixmap.size().width(),
                           sizeFactor*pixmap.size().height());
    QRectF source = pixmap.rect();
    painter->drawPixmap(target, pixmap, source);
    QPen pen = painter->pen();
    pen.setWidth(maxExpansion/32);
    pen.setColor(frameColor);
    painter->setPen(pen);
    maxExpansion = (maxExpansion - maxExpansion/32)/2;
    painter->drawEllipse(center,maxExpansion,maxExpansion);
  }
  /*
  else
  {
    // Draw pixmap in device coordinates to avoid pixmap scaling;
    QPixmap pixmap = sourcePixmap(Qt::DeviceCoordinates, &offset);
    painter->setWorldTransform(QTransform());
    QSizeF size = sourceBoundingRect().size();
    QPoint center = offset+QPoint(size.width()/2,size.height()/2);

    int maxExpansion = qMax(size.width(),size.height());
    qreal sizeFactor = (maxExpansion-maxExpansion/32)/qSqrt(size.height()*size.height()+size.width()*size.width());
    QRectF target = QRectF(center.x()-sizeFactor*size.width()/2,
                           center.y()-sizeFactor*size.height()/2,
                           sizeFactor*size.width(),
                           sizeFactor*size.height());
    QRectF source = pixmap.rect();
    painter->drawPixmap(target, pixmap, source);
    QPen pen = painter->pen();
    pen.setWidth(maxExpansion/32);
    painter->setPen(pen);
    maxExpansion = (maxExpansion - maxExpansion/32)/2;
    painter->drawEllipse(center,maxExpansion,maxExpansion);
  }
  */
}
