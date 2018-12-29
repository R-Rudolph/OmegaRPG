#ifndef NIGHTMODEDIALOG_H
#define NIGHTMODEDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QStackedLayout>
#include "widgets/nightmodeoverlay.h"

class NightModeDialog : public QDialog
{
  Q_OBJECT
  NightModeOverlay* overlay;
public:
  explicit NightModeDialog(QWidget *parent = nullptr);
  QWidget* getCentralWidget();
signals:

public slots:
};

#endif // NIGHTMODEDIALOG_H
