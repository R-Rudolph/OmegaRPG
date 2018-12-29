#ifndef GRAPHICSCIRCLEDEFFECT_H
#define GRAPHICSCIRCLEDEFFECT_H

#include <QObject>
#include <QGraphicsEffect>
#include <QPainter>

#include <QtMath>

class GraphicsCircledEffect : public QGraphicsEffect
{
  Q_OBJECT
  QColor frameColor;
public:
  explicit GraphicsCircledEffect(QObject *parent = 0);
  explicit GraphicsCircledEffect(const QColor& color, QObject *parent = 0);
  virtual QRectF boundingRectFor(const QRectF & rect) const;
  QColor getFrameColor() const;
  void setFrameColor(const QColor &value);

protected:
  virtual void draw(QPainter * painter);
signals:

public slots:
};

#endif // GRAPHICSCIRCLEDEFFECT_H
