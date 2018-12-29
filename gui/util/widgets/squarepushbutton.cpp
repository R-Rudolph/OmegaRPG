#include "squarepushbutton.h"
#include <QResizeEvent>

SquarePushButton::SquarePushButton(QWidget *parent) : QPushButton(parent)
{

}

SquarePushButton::SquarePushButton(const QString &text, QWidget *parent) : QPushButton(text, parent)
{

}

SquarePushButton::SquarePushButton(const QIcon &icon, const QString &text, QWidget *parent) : QPushButton(icon, text, parent)
{

}

SquarePushButton::SquarePushButton(QPushButtonPrivate &dd, QWidget *parent) : QPushButton(dd,parent)
{

}

int SquarePushButton::heightForWidth(int width) const
{
  return width;
}

void SquarePushButton::resizeEvent(QResizeEvent* event)
{
  int max = qMax(event->size().height(),event->size().width());
  resize(max,max);
}
