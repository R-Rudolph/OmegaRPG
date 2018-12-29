#include "consoleinterface.h"
#include "core/multi/util/global.h"

void ConsoleInterface::nameInterface()
{
  out << "Choose a name for your server." << endl;
  settings->serverName = in.readLine();
}

void ConsoleInterface::portInterface()
{
  while(1)
  {
    out << "Select a TCP port number for your server (default: " << Global::standardPort() << ")." << endl;
    out << "Leave empty to abort." << endl;
    QString input = in.readLine();
    if(input.isEmpty())
      break;
    int port = input.toInt();
    if(port>0 && port < 65536)
    {
      settings->port = port;
      break;
    }
    else
    {
      out << "Invalid port.";
    }
  }
}

void ConsoleInterface::adminPasswordInterface()
{
  out << "Select the new admin/lobby password." << endl;
  out << "Leave empty to prevent admin login." << endl;
  out << "Type 'cancel' to keep previous password." << endl;
  QString input = in.readLine();
  if(input!="cancel")
  {
    settings->setAdminPassword(input);
  }
}

void ConsoleInterface::domainInterface()
{
  out << "Type in your server's domain. Leave empty for automatic." << endl;
  settings->serverUrl = in.readLine();
}

void ConsoleInterface::registerInterface()
{
  while(1)
  {
    out << "Should your server register to the meta server? (default: yes)" << endl;
    out << "(y/n) ";
    out.flush();
    QString line = in.readLine();
    if(line=="y")
    {
      settings->notifyMetaServer = true;
      break;
    }
    else if(line=="n")
    {
      settings->notifyMetaServer = false;
      break;
    }
    else
    {
      out << "Invalid choice." << endl;
    }
  }

}

void ConsoleInterface::metaserverInterface()
{
  out << "Type in the meta server's address. Leave empty for default." << endl;
  settings->setCustomMetaServers(in.readLine());
}

void ConsoleInterface::settingsInterface()
{
  bool loop = true;
  while(loop)
  {
    QString name = settings->serverName.isEmpty() ? "None" : settings->serverName;
    QString reg = settings->notifyMetaServer ? "Yes" : "No";
    QString domain = settings->serverUrl.isEmpty() ? "Automatic" : settings->serverUrl;
    QString metaserver = settings->getCustomMetaServerString().isEmpty() ? "None" : settings->getCustomMetaServerString();
    out << "Please configure:" << endl;
    out << " 1) Server Name           (" << name                                             << ")" << endl;
    out << " 2) Server Port           (" << settings->port                                   << ")" << endl;
    out << " 3) Admin/Lobby Password  (" << QString(settings->getAdminPassword().size(),'*') << ")" << endl;
    out << " 4) Register Server       (" << reg                                              << ")" << endl;
    out << " 5) Server Domain         (" << domain                                           << ")" << endl;
    out << " 6) Add. Metaserver Addr. (" << metaserver                                       << ")" << endl;
    out << " 7) Certificate/Key       (" << shortCreditialErrorString()                      << ")" << endl;
    out << " 8) Done" << endl;
    QString input = in.readLine();
    int selection = input.toInt();
    switch(selection)
    {
      case 1:
        nameInterface();
        break;
      case 2:
        portInterface();
        break;
      case 3:
        adminPasswordInterface();
        break;
      case 4:
        registerInterface();
        break;
      case 5:
        domainInterface();
        break;
      case 6:
        metaserverInterface();
        break;
      case 7:
        certificateInterface();
        break;
      case 8:
      {
        orpg::ServerSettings::CredentialLoadingError error = settings->loadCredentials();
        if(error != orpg::ServerSettings::EverythingOk)
        {
          QSslCertificate cert;
          SslHelper::generateCredentials(cert,settings->privateKey,"DE","","nachtzone.duckdns.org","","","");
          SslHelper::saveCertificate(cert,settings->getCertificateFilepath());
          SslHelper::saveKey(settings->privateKey,settings->getPrivateKeyFilepath());
          QList<QSslCertificate> certChain;
          certChain.prepend(cert);
          settings->certificateChain = certChain;
        }
        loop = false;
        break;
      }
      default:
        out << "Invalid Selection." << endl;
        break;
    }
  }
  settings->save();
}

void ConsoleInterface::certificateInterface()
{
  bool loop = true;
  while(loop)
  {
    QString certPath = settings->getCertificateFilepath().isNull() ? "<Default>" : "("+settings->getCertificateFilepath()+")";
    QString keyPath = settings->getPrivateKeyFilepath().isNull() ? "<Default>" : "("+settings->getPrivateKeyFilepath()+")";
    out << "Certificate Status: " << certificateErrorString() << endl;
    out << "Private Key Status: " << keyErrorString() << endl;
    out << " 1) View Certificate" << endl;
    out << " 2) Set Certificate file path  " << certPath << endl;
    out << " 3) Set Private Key file path  " << keyPath << endl;
    out << " 4) Generate New Certificate"  << endl;
    out << " 5) Done"  << endl;
    QString input = in.readLine();
    int selection = input.toInt();
    switch(selection)
    {
      case 1:
        printCertificate();
        break;
      case 2:
      {
        QString certPath;
        stringInput("Enter your certificate's filepath.",certPath);
        settings->setCertificateFilepath(certPath);
        break;
      }
      case 3:
      {
        QString keyPath;
        stringInput("Enter your private key's filepath.",keyPath);
        settings->setPrivateKeyFilepath(keyPath);
        break;
      }
      case 4:
        generateCertificateInterface();
        break;
      case 5:
        loop = false;
        break;
      default:
        out << "Invalid Selection." << endl;
        break;
    }
  }
}

void ConsoleInterface::printSettings()
{
  QString name = settings->serverName.isEmpty() ? "<None>" : settings->serverName;
  QString reg = settings->notifyMetaServer ? "Yes" : "No";
  QString domain = settings->serverUrl.isEmpty() ? "<Automatic>" : settings->serverUrl;
  QString metaserver = settings->getCustomMetaServerString().isEmpty() ? "<None>" : settings->getCustomMetaServerString();
  QString keyFile = settings->getPrivateKeyFilepath().isEmpty() ? "<Default>" : settings->getPrivateKeyFilepath();
  QString certFile = settings->getCertificateFilepath().isEmpty() ? "<Default>" : settings->getCertificateFilepath();
  out << "Server Name:      " << name << endl;
  out << "Server Port:      " << settings->port << endl;
  out << "Register Server:  " << reg << endl;
  out << "Server Domain:    " << domain << endl;
  out << "Meta Server:      " << metaserver << endl;
  out << "Certificate File: " << certFile << endl;
  out << "Private Key File: " << keyFile << endl;
}

bool ConsoleInterface::generateCertificateInterface()
{
  QString keyPath = settings->getPrivateKeyFilepath();
  QString certPath = settings->getPrivateKeyFilepath();
  QString cn,o,ou,c,st,dn,sn,mail, l;
  int keyLength = 4096;
  bool loop = true;
  while(loop)
  {
    out << "Enter your information. " << endl;
    out << "  1) Common Name:        " << substitudeString(cn) << endl;
    out << "  2) Organization:       " << substitudeString(o) << endl;
    out << "  3) Organizational Unit:" << substitudeString(ou) << endl;
    out << "  4) Country:            " << substitudeString(c) << endl;
    out << "  5) State/Province:     " << substitudeString(st) << endl;
    out << "  6) Locality/City:      " << substitudeString(l) << endl;
    out << "  7) Certificate Path:   " << substitudeString(certPath,true) << endl;
    out << "  8) Private Key Path:   " << substitudeString(keyPath,true) << endl;
    out << "  9) RSA Key Length:     (" << keyLength << ")" << endl;
    out << " 10) Generate" << endl;
    out << " 11) Cancel" << endl;
    QString input = in.readLine();
    int selection = input.toInt();
    switch(selection)
    {
      case 1:
        stringInput("Enter the Common Name or Domain.",cn,false);
        break;
      case 2:
        stringInput("Enter the Organization.",o,false);
        break;
      case 3:
        stringInput("Enter the Organizational Unit.",ou,false);
        break;
      case 4:
        stringInput("Enter the Country.",c,false);
        break;
      case 5:
        stringInput("Enter the State or Province.",st,false);
        break;
      case 6:
        stringInput("Enter the Locality or City.",l,false);
        break;
      case 7:
        stringInput("Enter the file path the certificate will be saved to.",certPath,true);
        break;
      case 8:
        stringInput("Enter the file path the private key will be saved to.",keyPath,true);
        break;
      case 9:
        intInput("Enter the RSA Key Length in bits. Default is 4096.",keyLength,true,4096);
        break;
      case 10:
      {
        if(cn.isEmpty())
        {
          if(!yesNoInput("You didn't enter a common name. Continue anyway?"))
            break;
        }
        else
        {
          if(!yesNoInput("Are you sure?"))
            break;
        }
        if(QFile(certPath).exists() || QFile(keyPath).exists())
        {
          if(!yesNoInput("Certificate and/or Key file already exists. Overwrite?"))
            break;
        }
        QSslKey key;
        QSslCertificate cert;
        if(SslHelper::generateCredentials(cert,key,c,o,cn,ou,l,st) != SslHelper::ErrorNone)
        {
          out << "ERROR: Could not generate certificate." << endl;
          break;
        }
        QString pkBackup = settings->getPrivateKeyFilepath();
        QString certBackup = settings->getCertificateFilepath();
        settings->setPrivateKeyFilepath(keyPath);
        settings->setCertificateFilepath(certPath);
        if(!SslHelper::saveCertificate(cert,settings->getCertificateFilepath()))
        {
          settings->setPrivateKeyFilepath(pkBackup);
          settings->setCertificateFilepath(certBackup);
          out << "ERROR: Could not save certificate." << endl;
          break;
        }
        if(!SslHelper::saveKey(key,settings->getPrivateKeyFilepath()))
        {
          settings->setPrivateKeyFilepath(pkBackup);
          settings->setCertificateFilepath(certBackup);
          out << "ERROR: Could not save private key." << endl;
          break;
        }
        QList<QSslCertificate> certList;
        certList.prepend(cert);
        settings->certificateChain = certList;
        settings->privateKey = key;
        credentialError = orpg::ServerSettings::EverythingOk;
        out << "Certificate generated successfully." << endl;
        return true;
      }
      case 11:
        loop = false;
        break;
      default:
        out << "Invalid Choice." << endl;
        break;
    }
  }
  return false;
}

void ConsoleInterface::printCertificate()
{
  if(settings->certificateChain.isEmpty())
  {
    out << "CERTIFICATE ERROR: " << certificateErrorString() << endl << endl;
    return;
  }
  QSslCertificate certificate = settings->certificateChain.first();
  { //TO
    QStringList cn   = certificate.subjectInfo(QSslCertificate::CommonName);
    QStringList o    = certificate.subjectInfo(QSslCertificate::Organization);
    QStringList ou   = certificate.subjectInfo(QSslCertificate::OrganizationalUnitName);
    QStringList c    = certificate.subjectInfo(QSslCertificate::CountryName);
    QStringList st   = certificate.subjectInfo(QSslCertificate::StateOrProvinceName);
    QStringList dn   = certificate.subjectInfo(QSslCertificate::DistinguishedNameQualifier);
    QStringList sn   = certificate.subjectInfo(QSslCertificate::SerialNumber);
    QStringList mail = certificate.subjectInfo(QSslCertificate::EmailAddress);
    QStringList l    = certificate.subjectInfo(QSslCertificate::LocalityName);
    out << "Issued To:" << endl;
    if(!cn.isEmpty())
      out << " Common Name:          " << cn.join(", ") << endl;
    if(!o.isEmpty())
      out << " Organization:         " << o.join(", ") << endl;
    if(!ou.isEmpty())
      out << " Organizsational Unit: " << ou.join(", ") << endl;
    if(!c.isEmpty())
      out << " Country:              " << c.join(", ") << endl;
    if(!st.isEmpty())
      out << " State/Province:       " << st.join(", ") << endl;
    if(!l.isEmpty())
      out << " Locality/City:        " << l.join(", ") << endl;
    if(!dn.isEmpty())
      out << " Distinguished Name:   " << dn.join(", ") << endl;
    if(!sn.isEmpty())
      out << " Serial Number:        " << sn.join(", ")  << endl;
    if(!mail.isEmpty())
      out << " E-Mail Address:       " << mail.join(", ") << endl;
  }
  out << endl;
  { //FROM
    if(certificate.isSelfSigned())
    {
      out << "The Certificate is self-signed." << endl;
    }
    else
    {
      QStringList cn   = certificate.issuerInfo(QSslCertificate::CommonName);
      QStringList o    = certificate.issuerInfo(QSslCertificate::Organization);
      QStringList ou   = certificate.issuerInfo(QSslCertificate::OrganizationalUnitName);
      QStringList c    = certificate.issuerInfo(QSslCertificate::CountryName);
      QStringList st   = certificate.issuerInfo(QSslCertificate::StateOrProvinceName);
      QStringList dn   = certificate.issuerInfo(QSslCertificate::DistinguishedNameQualifier);
      QStringList sn   = certificate.issuerInfo(QSslCertificate::SerialNumber);
      QStringList mail = certificate.issuerInfo(QSslCertificate::EmailAddress);
      QStringList l    = certificate.issuerInfo(QSslCertificate::LocalityName);
      out << "Issued By:" << endl;
      if(!cn.isEmpty())
        out << " Common Name:          " << cn.join(", ") << endl;
      if(!o.isEmpty())
        out << " Organization:         " << o.join(", ") << endl;
      if(!ou.isEmpty())
        out << " Organizsational Unit: " << ou.join(", ") << endl;
      if(!c.isEmpty())
        out << " Country:              " << c.join(", ") << endl;
      if(!st.isEmpty())
        out << " State/Province:       " << st.join(", ") << endl;
      if(!dn.isEmpty())
        out << " Distinguished Name:   " << dn.join(", ") << endl;
      if(!sn.isEmpty())
        out << " Serial Number:        " << sn.join(", ")  << endl;
      if(!mail.isEmpty())
        out << " E-Mail Address:       " << mail.join(", ") << endl;
    }
  }
  out << endl;
  { //PERIOD OF VALIDITY
    out << "Period of Validity:" << endl;
    out << certificate.effectiveDate().toString() << endl;
    out << certificate.expiryDate().toString() << endl;
  }
  out << endl;
  { //FINGERPRINTS
    out << "Fingerprints:" << endl;
    out << "SHA1:    " << hexString(certificate.digest(QCryptographicHash::Sha1))   << endl;
    out << "SHA-256: " << hexString(certificate.digest(QCryptographicHash::Sha256)) << endl;
  }
  out << endl << "Press Enter to continue." << endl;
  out.flush();
  in.readLine();
}

void ConsoleInterface::loadCertificateAndKey()
{
  credentialError = settings->loadCredentials();
}

void ConsoleInterface::stringInput(const QString &message, QString &dest, bool showDefault, const QString& defaultValue)
{
  out << message << endl;
  out << "Enter 'cancel' to abort." << endl;
  if(showDefault)
    out << "Leave empty for default." << endl;
  QString input = in.readLine();
  if(input=="cancel")
    return;
  if(input.isEmpty() && showDefault)
    input = defaultValue;
  dest = input;
}

void ConsoleInterface::intInput(const QString &message, int &dest, bool showDefault, int defaultValue)
{
  bool loop = true;
  while(loop)
  {
    out << message << endl;
    out << "Enter 'cancel' to abort." << endl;
    if(showDefault)
      out << "Leave empty for default." << endl;
    QString input = in.readLine();
    if(input=="cancel")
      return;
    bool ok;
    int value = input.toInt(&ok);
    if(input.isEmpty() && showDefault)
    {
      value = defaultValue;
      ok = true;
    }
    if(!ok)
    {
      out << "Invalid value." << endl;
      continue;
    }
    dest = value;
    break;
  }
}

bool ConsoleInterface::yesNoInput(const QString &message)
{
  while(true)
  {
    out << message << endl;
    out << "(y/n) ";
    out.flush();
    QString input = in.readLine();
    if(input=="y")
      return true;
    else if(input=="n")
      return false;
    out << "Invalid input." << endl;
  }
  return false;
}

QString ConsoleInterface::shortCreditialErrorString()
{
  if(credentialError==orpg::ServerSettings::EverythingOk)
    return "Ok";
  else
    return "Error";
}

QString ConsoleInterface::keyErrorString()
{
  orpg::ServerSettings::CredentialLoadingError error;
  error = (orpg::ServerSettings::CredentialLoadingError) ( credentialError & (
        orpg::ServerSettings::KeyFileDoesNotExist |
        orpg::ServerSettings::KeyFileInvalid |
        orpg::ServerSettings::KeyFileNotOpened));
  switch(error)
  {
    case orpg::ServerSettings::KeyFileDoesNotExist:
      return "File does not exist.";
      break;
    case orpg::ServerSettings::KeyFileInvalid:
      return "File is invalid";
      break;
    case orpg::ServerSettings::KeyFileNotOpened:
      return "Could not open file";
      break;
    default:
      return "Ok";
      break;
  }
}

QString ConsoleInterface::certificateErrorString()
{
  orpg::ServerSettings::CredentialLoadingError error;
  error = (orpg::ServerSettings::CredentialLoadingError) ( credentialError & (
        orpg::ServerSettings::CertFileDoesNotExist |
        orpg::ServerSettings::CertFileInvalid |
        orpg::ServerSettings::CertFileNotOpened));
  switch(error)
  {
    case orpg::ServerSettings::CertFileDoesNotExist:
      return "File does not exist.";
      break;
    case orpg::ServerSettings::CertFileInvalid:
      return "File invalid";
      break;
    case orpg::ServerSettings::CertFileNotOpened:
      return "Could not open file";
      break;
    default:
      return "Ok";
      break;
  }
}

QString ConsoleInterface::hexString(const QByteArray &array, int lineLength)
{
  QStringList list;
  QString hexString = QString::fromLatin1(array.toHex());
  for(int i=0;i<hexString.size();i=i+2)
  {
    list.append(hexString.mid(i,2));
  }
  QString result = list.join(":").toUpper();
  if(lineLength!=0)
  {
    QString result2;
    for(int i=0;i<result.length();i+=lineLength)
    {
      result2.append(result.mid(i,lineLength));
      result2.append("\n");
    }
    result = result2;
  }
  return result;
}

QString ConsoleInterface::substitudeString(const QString &string, bool defaultNotEmpty)
{
  if(string.isEmpty())
  {
    if(defaultNotEmpty)
      return "<Default>";
    else
      return "<Empty>";
  }
  return "("+string+")";
}

ConsoleInterface::ConsoleInterface(QObject *parent)
  : QObject(parent), in(stdin), out(stdout)
{
  settings = new orpg::ServerSettings(this);
  server = new orpg::NetworkServer(this,*settings);
  delete settings;
  settings = server->getSettings();
}

bool ConsoleInterface::init(bool forceConfig)
{
  bool configured = false;
  if(!settings->load() || forceConfig)
  {
    configured = true;
    settingsInterface();
  }
  if(server->openServer())
  {
    out << "Server lauched successfully." << endl;
    if(!configured)
      printSettings();
    return true;
  }
  else
  {
    out << "Error! Could not bind port." << endl;
    out << "run " << endl;
    out << "  OpenRPG-Server --config" << endl;
    out << "if you want to change the port (currently " << settings->port << ")" << endl;
    return false;
  }
}

void ConsoleInterface::printHelp()
{
  out << "Usage: OpenRPG-Server [OPTION]" << endl << endl;
  out << "Options:" << endl;
  out << "  -c, --config   Shows the configuration dialog" << endl;
  out << "  -h, --help   Shows this message" << endl << endl;
  out << "OmegaRPG is a virtual game table application powered by Qt.";
}
