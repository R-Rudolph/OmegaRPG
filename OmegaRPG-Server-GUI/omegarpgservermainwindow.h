#ifndef OMEGARPGSERVERMAINWINDOW_H
#define OMEGARPGSERVERMAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
#include "widgets/servercontrolwidget.h"
#include "core/multi/util/global.h"
#include "gui/util/globalgui.h"

class OmegaRpgServerMainWindow : public QMainWindow
{
  Q_OBJECT
  ServerControlWidget* serverWidget;
public:
  OmegaRpgServerMainWindow(QWidget *parent = 0);
  ~OmegaRpgServerMainWindow();
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // OMEGARPGSERVERMAINWINDOW_H
