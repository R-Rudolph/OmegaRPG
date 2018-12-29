#include "omegarpgservermainwindow.h"
#include <QApplication>
#include "core/multi/util/global.h"
#include "gui/util/globalgui.h"
#include <QDir>

int main(int argc, char *argv[])
{
  Global::createEnvironment();
  QApplication a(argc, argv);
  a.setWindowIcon(GlobalGUI::getOmegaRPGIcon());
  OmegaRpgServerMainWindow w;
  w.setWindowTitle("OmegaRPG Server");
  w.show();

  return a.exec();
}
