#include "colorselectbutton.h"

QString ColorSelectButton::getStyleSheet(const QColor &color)
{
  return
 "ColorSelectButton \
  { \
    border-width: 1px; \
    border-style: solid; \
    border-radius: 6px; \
    background-color: "+color.name()+"; \
  }";
}

QString ColorSelectButton::getStyleSheet(const QColor &color, const QColor& bcolor)
{
  return
 "ColorSelectButton \
  { \
    border-color: "+bcolor.name()+"; \
    border-width: 1px; \
    border-style: solid; \
    border-radius: 6px; \
    background-color: "+color.name()+"; \
  }";
}

ColorSelectButton::ColorSelectButton(QWidget *parent, const QColor& color) : QFrame(parent)
{
  this->setAutoFillBackground(true);
  this->setMinimumHeight(25);
  this->setMaximumHeight(25);
  this->setMinimumWidth(35);
  colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog,true);

  connect(colorDialog,&QColorDialog::currentColorChanged,this,&ColorSelectButton::setCurrentColor);
  connect(colorDialog,&QColorDialog::colorSelected,this,qOverload<const QColor&>(&ColorSelectButton::setFinalColor));
  connect(colorDialog,&QDialog::rejected,this,&ColorSelectButton::resetColor);
  this->setObjectName("ColorSelectButton");
  setFinalColor(color);
}

void ColorSelectButton::setFinalColor(const QColor &color, bool emitSignal)
{
  this->color = color;
  this->colorBackup = color;
  QPalette pal;
  pal.setColor(QPalette::Background,color);
  this->setPalette(pal);
  this->setStyleSheet(getStyleSheet(color));
  colorDialog->setCurrentColor(color);
  if(emitSignal)
  {
    emit colorChanged(color);
    emit finalColorChanged(color);
  }
}

void ColorSelectButton::setCurrentColor(const QColor &color)
{
  this->color = color;
  QPalette pal;
  pal.setColor(QPalette::Background,color);
  this->setPalette(pal);
  this->setStyleSheet(getStyleSheet(color));
  emit colorChanged(color);
}

void ColorSelectButton::resetColor()
{
  setFinalColor(colorBackup);
}


void ColorSelectButton::setColor(const QColor& color, bool emitSignal)
{
  setFinalColor(color,emitSignal);
}

void ColorSelectButton::setDisabled(bool value)
{
  colorDialog->close();
  resetColor();
  if(value)
    this->setStyleSheet(getStyleSheet(QColor(230,230,230),QColor(180,180,180)));
  QFrame::setDisabled(value);
}

void ColorSelectButton::closeDialog()
{
  colorDialog->close();
}

void ColorSelectButton::mouseReleaseEvent(QMouseEvent *event)
{
  if(event->button()==Qt::LeftButton)
  {
    colorDialog->show();
    event->accept();
  }
  else if(event->button()==Qt::RightButton)
    emit rightClicked();
}

void ColorSelectButton::setFinalColor(const QColor &color)
{
  setFinalColor(color,true);
}

QColor ColorSelectButton::getColor()
{
  return colorBackup;
}

QColor ColorSelectButton::getCurrentColor()
{
  return color;
}
