#include <QCoreApplication>
#include <QSet>
#include "consoleinterface.h"
#include <QDir>
#include "core/multi/util/global.h"
#include<QtGlobal>

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  Global::createEnvironment();
  #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
  QSet<QString> args(a.arguments().begin(),a.arguments().end());
  #else
  QSet<QString> args = a.arguments().toSet();
  #endif
  ConsoleInterface cli;
  int retcode = -1;
  if(args.contains("-h") || args.contains("--help"))
  {
    cli.printHelp();
    retcode = 0;
  }
  else if(args.contains("-c") || args.contains("--config"))
  {
    if(!cli.init(true))
      retcode = 1;
  }
  else
  {
    if(!cli.init(false))
      retcode = 1;
  }
  if(retcode==-1)
    return a.exec();
  else
    return retcode;
}
