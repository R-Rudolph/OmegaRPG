#ifndef OMEGARPG_GLOBAL_H
#define OMEGARPG_GLOBAL_H

#include <QString>
#include <QStringList>
#include "core/multi/version.h"

class Global
{
  static bool initializedDir;
  static bool useDocumentDir;
  static bool disIsWritable(const QString& path);
public:
  static QString dataFolder();
  static QString imageCacheFolder();
  static QString logFolder();
  static QString openRpgLogFolder();
  static QString settingsFolder();
  static QString sslFolder();
  static QString pluginsFolder();

  static QString settingsFilepath();
  static QString lobbyFilepath();
  static QString serverSettingsFilepath();
  static QString windowSettingsFilepath();
  static QString aliasLibFilepath();
  static QString miniLibFilepath();
  static QString cacheIndexFilepath();
  static QString favouritesFilepath();
  static QString historyFilepath();
  static QString sslSettingsFilepath();
  static QString sslPrivateKeyFilepathDefault();
  static QString sslCertificateFilepathDefault();

  static QStringList getStaticMetaServerUrls();
  static int standardPort();
  static QString imgurClientID();

  static orpg::Version applicationVersion();
  static orpg::Version protocolVersion();

  static void createEnvironment();
};


#endif // OMEGARPG_GLOBAL_H
