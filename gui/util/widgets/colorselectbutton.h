#ifndef COLORSELECTBUTTON_H
#define COLORSELECTBUTTON_H

#include <QWidget>
#include <QMouseEvent>

#include <QColorDialog>
#include <QFrame>
#include <QVBoxLayout>

class ColorSelectButton : public QFrame
{
  Q_OBJECT
  QColor color;
  QColor colorBackup;
  QColorDialog* colorDialog;

  QFrame* frame;
  QString getStyleSheet(const QColor& color);
  QString getStyleSheet(const QColor& color, const QColor& bcolor);
public:
  explicit ColorSelectButton(QWidget *parent = 0, const QColor& color = Qt::red);
  QColor getColor();
  QColor getCurrentColor();
  void setColor(const QColor& color, bool emitSignal=true);
  void setColorExternal(const QColor& color);
  void setDisabled(bool value);
  void closeDialog();
protected:
  void mouseReleaseEvent (QMouseEvent * event);
signals:
  void colorChanged(QColor color);
  void finalColorChanged(QColor color);
  void rightClicked();
private slots:
  void setFinalColor(const QColor& color);
  void setFinalColor(const QColor& color, bool emitSignal);
  void setCurrentColor(const QColor& color);
  void resetColor();
};

#endif // COLORSELECTBUTTON_H
