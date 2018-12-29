#include "global.h"
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>

bool Global::initializedDir = false;
bool Global::useDocumentDir = false;

bool Global::disIsWritable(const QString &path)
{
  QDir dir(path);
  QString filepath = dir.absoluteFilePath(".writeTest");
  QFile file(filepath);
  if(file.open(QIODevice::WriteOnly))
  {
    file.close();
    QFile::remove(filepath);
    return true;
  }
  return false;
}

QString Global::dataFolder()
{
  #ifdef _WIN32
  if(!initializedDir)
  {
    useDocumentDir = !Global::disIsWritable("./");
    initializedDir = true;
  }
  if(useDocumentDir)
    return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/OmegaRPG/";
  else
    return "./";
  #else
  return QDir::homePath()+"/.OmegaRPG/";
  #endif
}

QString Global::imageCacheFolder()
{
  return dataFolder()+"imagecache/";
}

QString Global::logFolder()
{
  return dataFolder()+"logs/";
}

QString Global::settingsFilepath()
{
  return settingsFolder()+"orpg_settings.json";
}

QString Global::lobbyFilepath()
{
  return settingsFolder()+"lobby.json";
}

QString Global::aliasLibFilepath()
{
  return settingsFolder()+"orpg_aliaslib.json";
}

QString Global::miniLibFilepath()
{
  return settingsFolder()+"orpg_minilib.json";
}

QString Global::cacheIndexFilepath()
{
  return settingsFolder()+"orpg_cache.json";
}

QString Global::settingsFolder()
{
  return dataFolder()+"settings/";
}

QString Global::favouritesFilepath()
{
  return settingsFolder()+"orpg_favourites.json";
}

QString Global::historyFilepath()
{
  return settingsFolder()+"orpg_history.json";
}

QStringList Global::getStaticMetaServerUrls()
{
  return QStringList({"https://night.elnath.uberspace.de/OmegaRPG"});
  //return "omegarpg.vastserve.com";
}

QString Global::serverSettingsFilepath()
{
  return settingsFolder()+"orpg_serversettings.json";
}

int Global::standardPort()
{
  return 7001;
}

QString Global::imgurClientID()
{
  return "8ece3ddc5879283";
}

orpg::Version Global::applicationVersion()
{
  return orpg::Version(APP_VERSION);
}

orpg::Version Global::protocolVersion()
{
  return orpg::Version(PROTOCOL_VERSION);
}

void Global::createEnvironment()
{
  QDir dir;
  dir.mkpath(Global::dataFolder());
  dir.mkpath(Global::settingsFolder());
  dir.mkpath(Global::imageCacheFolder());
  dir.mkpath(Global::logFolder());
  dir.mkpath(Global::sslFolder());
  dir.mkpath(Global::pluginsFolder());
}

QString Global::windowSettingsFilepath()
{
  return settingsFolder()+"orpg_windowsettings.ini";
}

QString Global::sslSettingsFilepath()
{
  return settingsFolder()+"orpg_ssl.json";
}

QString Global::sslFolder()
{
  return dataFolder()+"ssl/";
}

QString Global::pluginsFolder()
{
  return dataFolder()+"plugins/";
}

QString Global::sslPrivateKeyFilepathDefault()
{
  return sslFolder()+"key.pem";
}

QString Global::sslCertificateFilepathDefault()
{
  return sslFolder()+"certificate.pem";
}

QString Global::openRpgLogFolder()
{
  return dataFolder()+"OpenRPG-Logs/";
}
