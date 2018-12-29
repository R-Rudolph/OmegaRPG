#include "coloroverlaywrapperwidget.h"

ColorOverlayWrapperWidget::ColorOverlayWrapperWidget(QWidget *parent, Qt::WindowFlags f) : QWidget(parent,f)
{
  layout = new QStackedLayout(this);
  layout->setStackingMode(QStackedLayout::StackAll);
  layout->setMargin(0);
  overlay = new ColorOverlayWidget(this);
  layout->addWidget(overlay);
}

void ColorOverlayWrapperWidget::setWidget(QWidget *widget)
{
  layout->addWidget(widget);
  layout->setCurrentWidget(overlay);
  this->widget = widget;
}

QWidget *ColorOverlayWrapperWidget::getWidget() const
{
  return widget;
}

void ColorOverlayWrapperWidget::setColor(const QColor &color)
{
  overlay->setColor(color);
}
