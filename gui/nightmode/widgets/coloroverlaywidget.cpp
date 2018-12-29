#include "coloroverlaywidget.h"

QColor ColorOverlayWidget::getColor() const
{
  return color;
}

void ColorOverlayWidget::setColor(const QColor &value)
{
  color = value;
  update();
}

void ColorOverlayWidget::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  painter.fillRect(event->rect(),color);
}

ColorOverlayWidget::ColorOverlayWidget(QWidget *parent) : QWidget(parent)
{
  color = QColor(0,0,0,0);
  setAttribute(Qt::WA_NoSystemBackground);
  setAttribute(Qt::WA_TransparentForMouseEvents);
}
