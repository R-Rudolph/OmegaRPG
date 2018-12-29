#ifndef NIGHTMODE_H
#define NIGHTMODE_H

#include <QCoreApplication>
#include <QApplication>
#include <QObject>
#include <QColor>

class NightModeController : public QObject
{
  Q_OBJECT
  bool nightMode;
  bool nightModeWorkaround;
  QColor overlayColor;
  static NightModeController* globalNMC;
  explicit NightModeController(QColor overlayColor, QObject *parent = 0);
  explicit NightModeController(QObject *parent = 0);
public:
  bool getNightMode() const;
  QColor getOverlayColor() const;
  static NightModeController* get();
signals:
  void nightModeChanged(bool value);
public slots:
  void setNightMode(bool value);
  void nightModeOn();
  void nightModeOff();
  void toggle();
  void setOverlayColor(const QColor &value);
  void setNightModeWorkaround(bool value);
};


#endif // NIGHTMODE_H
