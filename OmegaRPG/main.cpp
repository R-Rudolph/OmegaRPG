#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <QFileInfo>

#include "omegarpgmainwindow.h"
#include "gui/util/globalgui.h"

#include "core/multi/roomvariables/mapsettingsvariablecontainer.h"
#include "core/multi/roomvariables/mapsettingsvariable.h"

#include "core/client/log/htmllogparser.h"
#include "core/client/log/jsonlogparser.h"

void importOldLogs()
{
  QDir dir(orpg::Settings::get()->getLogLocation());
  QStringList htmls = dir.entryList(QStringList("*.html"),QDir::Files,QDir::Name|QDir::IgnoreCase);
  QStringList jsons = dir.entryList(QStringList("*.json"),QDir::Files,QDir::Name|QDir::IgnoreCase);
  foreach(QString file, htmls)
  {
    QString jsonFilename = file;
    jsonFilename.chop(4);
    jsonFilename.append("json");
    if(!jsons.contains(jsonFilename))
    { 
      orpg::HtmlLogParser parse(dir.absoluteFilePath(file));
      if(parse.isValid())
      {
        QFile jsonFile(dir.absoluteFilePath(jsonFilename));
        if(jsonFile.open(QIODevice::WriteOnly))
        {
          jsonFile.write(QJsonDocument(parse.toJson()).toJson(QJsonDocument::Compact));
          jsonFile.close();
        }
      }
    }
  }
}

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  Global::createEnvironment();
  GlobalGUI::initIcons(app);
  app.setWindowIcon(GlobalGUI::getOmegaRPGIconSmall());

  OmegaRpgMainWindow orpg;
  orpg.show();
  orpg.connectionWindow->show();

  importOldLogs();
  return app.exec();
}
