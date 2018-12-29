#ifndef NIGHTMODEOVERLAY_H
#define NIGHTMODEOVERLAY_H

#include <QWidget>
#include "widgets/coloroverlaywrapperwidget.h"

class NightModeOverlay : public ColorOverlayWrapperWidget
{
  Q_OBJECT
public:
  explicit NightModeOverlay(QWidget *parent = 0, Qt::WindowFlags f=0);
signals:

private slots:
  void updateOverlay(bool);
};

#endif // NIGHTMODEOVERLAY_H
