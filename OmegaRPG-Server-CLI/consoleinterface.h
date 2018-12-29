#ifndef CONSOLEINTERFACE_H
#define CONSOLEINTERFACE_H

#include <iostream>
#include <QObject>
#include <QTextStream>
#include <QFile>

#include "core/server/serversettings.h"
#include "core/server/networkserver.h"

class ConsoleInterface : public QObject
{
  Q_OBJECT
  QTextStream in;
  QTextStream out;

  orpg::ServerSettings* settings;
  orpg::NetworkServer* server;

  orpg::ServerSettings::CredentialLoadingError credentialError;

  void nameInterface();
  void portInterface();
  void adminPasswordInterface();
  void domainInterface();
  void registerInterface();
  void metaserverInterface();
  void settingsInterface();
  void certificateInterface();
  bool generateCertificateInterface();

  void printSettings();
  void printCertificate();
  void loadCertificateAndKey();
  void stringInput(const QString& message, QString& dest, bool showDefault = true, const QString& defaultValue="");
  void intInput(const QString& message, int& dest, bool showDefault=false, int defaultValue=0);
  bool yesNoInput(const QString& message);
  QString shortCreditialErrorString();
  QString keyErrorString();
  QString certificateErrorString();
  QString hexString(const QByteArray &array, int lineLength=0);
  QString substitudeString(const QString& string, bool defaultNotEmpty=false);
public:
  explicit ConsoleInterface(QObject *parent = 0);
  bool init(bool forceConfig);
  void printHelp();
signals:

public slots:
};

#endif // CONSOLEINTERFACE_H
