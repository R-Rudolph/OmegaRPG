#include <QCoreApplication>
#include <QSet>
#include "consoleinterface.h"
#include <QDir>
#include "core/multi/util/global.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  Global::createEnvironment();
  QSet<QString> args = a.arguments().toSet();
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
