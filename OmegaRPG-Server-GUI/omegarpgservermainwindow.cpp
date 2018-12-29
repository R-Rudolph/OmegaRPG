#include "omegarpgservermainwindow.h"

OmegaRpgServerMainWindow::OmegaRpgServerMainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  serverWidget = new ServerControlWidget(this);
  setCentralWidget(serverWidget);
}

OmegaRpgServerMainWindow::~OmegaRpgServerMainWindow()
{

}

void OmegaRpgServerMainWindow::closeEvent(QCloseEvent *event)
{
  QMessageBox msgBox;
  msgBox.setWindowIcon(this->windowIcon());
  msgBox.setText("Do you want to close the OmegaRPG Server?");
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::Yes);
  int ret = msgBox.exec();
  switch(ret)
  {
    case QMessageBox::Yes:
      event->accept();
      break;
    case QMessageBox::No:
      event->ignore();
      break;
    default:
      break;
  }
}
