#ifndef COLOROVERLAYWIDGET_H
#define COLOROVERLAYWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QColor>
#include <QPainter>

class ColorOverlayWidget : public QWidget
{
  Q_OBJECT
  QColor color;
public:
  explicit ColorOverlayWidget(QWidget *parent = 0);
  QColor getColor() const;
  void setColor(const QColor &value);
protected:
  virtual void paintEvent(QPaintEvent *event);
signals:

public slots:
};

#endif // COLOROVERLAYWIDGET_H
