#ifndef COLOREDOVERLAYWIDGET_H
#define COLOREDOVERLAYWIDGET_H

#include <QWidget>
#include <QStackedLayout>
#include "coloroverlaywidget.h"

class ColorOverlayWrapperWidget : public QWidget
{
  Q_OBJECT
  QStackedLayout* layout;
  ColorOverlayWidget* overlay;
  QWidget* overlayed;
  QWidget* widget;
public:
  explicit ColorOverlayWrapperWidget(QWidget *parent = 0, Qt::WindowFlags f=0);
  void setWidget(QWidget* widget);
  QWidget* getWidget() const;
signals:

public slots:
  void setColor(const QColor& color);
};

#endif // COLOREDOVERLAYWIDGET_H
