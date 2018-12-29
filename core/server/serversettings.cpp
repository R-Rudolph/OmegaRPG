#include "serversettings.h"
#include <QFileSystemWatcher>
#include <QMetaType>
#include <QSaveFile>
#include <QFile>
#include <QJsonDocument>
#include <QTimer>
#include "core/multi/util/global.h"
#include "util/qtio.h"
#include "util/sslhelper.h"

namespace orpg
{
  bool ServerSettings::typesRegistered = false;

  void ServerSettings::registerTypes()
  {
    if(typesRegistered)
      return;
    typesRegistered = true;
    qRegisterMetaType<orpg::ServerSettings>("orpg::ServerSettings");
    qRegisterMetaType<orpg::ServerSettings*>("orpg::ServerSettings*");
  }

  void ServerSettings::setStandardValues()
  {
    autoUpdateCredentials = true;
    serverName = QString();
    serverUrl = QString();
    notifyMetaServer = true;
    metaServerHeartbeat = 3600000;
    port = Global::standardPort();
  }

  bool ServerSettings::parse(const QJsonObject &serverSetttingsObject)
  {
    if(serverSetttingsObject["type"]=="OmegaRPG Server Settings File")
    {
      setCustomMetaServers(serverSetttingsObject["MetaServer"].toString(""));
      serverName = serverSetttingsObject["ServerName"].toString(serverName);
      serverUrl = serverSetttingsObject["ServerUrl"].toString(serverUrl);
      metaServerHeartbeat = serverSetttingsObject["MetaServerHeartbeat"].toInt(metaServerHeartbeat);
      notifyMetaServer = serverSetttingsObject["NofityMetaServer"].toBool(notifyMetaServer);
      port = serverSetttingsObject["Port"].toInt(port);
      setPrivateKeyFilepath(serverSetttingsObject["KeyFilepath"].toString());
      setCertificateFilepath(serverSetttingsObject["CertificateFilepath"].toString());
      serverMessage = serverSetttingsObject["serverMessage"].toString();
      adminPassword = serverSetttingsObject["adminPassword"].toString();
      return true;
    }
    return false;
  }

  QJsonObject ServerSettings::toJson()
  {
    QJsonObject result;
    result.insert("type","OmegaRPG Server Settings File");
    result.insert("MetaServer",getCustomMetaServerString());
    result.insert("ServerName",serverName);
    result.insert("ServerUrl",serverUrl);
    result.insert("MetaServerHeartbeat",metaServerHeartbeat);
    result.insert("NofityMetaServer",notifyMetaServer);
    result.insert("Port",port);
    result.insert("KeyFilepath",privateKeyFilepath);
    result.insert("CertificateFilepath",certificateFilepath);
    result.insert("serverMessage",serverMessage);
    result.insert("adminPassword",adminPassword);
    return result;
  }

  ServerSettings::CredentialLoadingError ServerSettings::loadKey()
  {
    if(getPrivateKeyFilepath().isEmpty())
    {
      return ServerSettings::KeyFileDoesNotExist;
    }
    QFile file(getPrivateKeyFilepath());
    if(!file.exists())
    {
      QtIO::err << "Could not load Private Key '" << getPrivateKeyFilepath() << "'. File does not exist." << endl;
      return ServerSettings::KeyFileDoesNotExist;
    }
    else if(file.open(QIODevice::ReadOnly))
    {
      privateKey = SslHelper::loadKey(&file,QSsl::PrivateKey);
      file.close();
      if(privateKey.isNull())
      {
        QtIO::err << "Could not load Private Key '" << getPrivateKeyFilepath() << "'. File is invalid." << endl;
        return ServerSettings::KeyFileInvalid;
      }
      emit privateKeyChanged(privateKey);
      return ServerSettings::EverythingOk;
    }
    else
    {
      QtIO::err << "Could not load Private Key '" << getPrivateKeyFilepath() << "'. Could not open file." << endl;
      return ServerSettings::KeyFileNotOpened;
    }
  }

  ServerSettings::CredentialLoadingError ServerSettings::loadCert()
  {
    if(getCertificateFilepath().isEmpty())
    {
      return ServerSettings::CertFileDoesNotExist;
    }
    QFile file(getCertificateFilepath());
    if(!file.exists())
    {
      QtIO::err << "Could not load Certificate '" << getCertificateFilepath() << "'. File does not exist" << endl;
      return ServerSettings::CertFileDoesNotExist;
    }
    else if(file.open(QIODevice::ReadOnly))
    {
      certificateChain = QSslCertificate::fromDevice(&file,QSsl::Pem);
      file.close();
      if(certificateChain.isEmpty())
      {
        QtIO::err << "Could not load Certificate '" << getCertificateFilepath() << "'. File invalid." << endl;
        return ServerSettings::CertFileInvalid;
      }
      emit certificateChainChanged(certificateChain);
      return ServerSettings::EverythingOk;
    }
    else
    {
      QtIO::err << "Could not load Certificate '" << getCertificateFilepath() << "'. Could not open file.." << endl;
      return ServerSettings::CertFileNotOpened;
    }
  }

  ServerSettings::ServerSettings(QObject *parent) : QObject(parent)
  {
    registerTypes();
    staticMetaServers = Global::getStaticMetaServerUrls();
    setStandardValues();
    credentialWatcher = new QFileSystemWatcher(this);
    connect(credentialWatcher,&QFileSystemWatcher::fileChanged,this,&ServerSettings::credentialChanged);
    tryingLater = false;
  }

  ServerSettings::ServerSettings(const ServerSettings &other, QObject *parent) : ServerSettings(parent)
  {
    this->set(other);
  }
  
  QString ServerSettings::getAdminPassword() const
  {
    return adminPassword;
  }
  
  void ServerSettings::setAdminPassword(const QString& value)
  {
    adminPassword = value;
  }
  
  void ServerSettings::setCustomMetaServers(const QString &servers)
  {
    customMetaServers = servers.split(" ");
  }
  
  QStringList ServerSettings::getMetaServers()
  {
    QStringList list = staticMetaServers;
    foreach(const QString& server, synchedMetaServers)
    {
      if(!list.contains(server))
        list.append(server);
    }
    foreach(const QString& server, customMetaServers)
    {
      if(!list.contains(server))
        list.append(server);
    }
    return list;
  }

  QString ServerSettings::getCustomMetaServerString()
  {
    return customMetaServers.join(" ");
  }

  QString ServerSettings::getCertificateFilepath()
  {
    return certificateFilepath;
  }

  QString ServerSettings::getPrivateKeyFilepath()
  {
    return privateKeyFilepath;
  }

  void ServerSettings::setCertificateFilepath(const QString &certificateFilepath)
  {
    if(!this->certificateFilepath.isEmpty())
    {
      credentialWatcher->removePath(this->certificateFilepath);
    }
    this->certificateFilepath = certificateFilepath;
    if(!tryingLater)
      fillCredentialWatcher();
  }

  void ServerSettings::setPrivateKeyFilepath(const QString &privateKeyFilepath)
  {
    if(!this->privateKeyFilepath.isEmpty())
    {
      credentialWatcher->removePath(this->privateKeyFilepath);
    }
    this->privateKeyFilepath = privateKeyFilepath;
    if(!tryingLater)
      fillCredentialWatcher();
  }

  bool ServerSettings::load(const QString &filename)
  {
    QFile file;
    if(filename.isEmpty())
      file.setFileName(Global::serverSettingsFilepath());
    else
      file.setFileName(filename);
    if(file.open(QIODevice::ReadOnly))
    {
      QJsonParseError error;
      QJsonDocument doc = QJsonDocument::fromJson(file.readAll(),&error);
      file.close();
      if(error.error!=QJsonParseError::NoError)
        return false;
      return parse(doc.object());
    }
    return false;
  }

  bool ServerSettings::save(const QString &filename)
  {
    QSaveFile file;
    if(filename.isEmpty())
      file.setFileName(Global::serverSettingsFilepath());
    else
      file.setFileName(filename);
    if(file.open(QIODevice::WriteOnly))
    {
      QJsonDocument doc(toJson());
      file.write(doc.toJson());
      file.commit();
      return true;
    }
    return false;
  }

  ServerSettings::CredentialLoadingError ServerSettings::loadCredentials()
  {
    CredentialLoadingError keyOk = loadKey();
    CredentialLoadingError certOk = loadCert();
    return (CredentialLoadingError) (keyOk | certOk);
  }

  void ServerSettings::set(const ServerSettings &other)
  {
    this->notifyMetaServer = other.notifyMetaServer;
    this->staticMetaServers = other.staticMetaServers;
    this->synchedMetaServers = other.synchedMetaServers;
    this->customMetaServers = other.customMetaServers;
    this->serverName = other.serverName;
    this->serverUrl = other.serverUrl;
    this->metaServerHeartbeat = other.metaServerHeartbeat;
    this->port = other.port;
    this->adminPassword = other.getAdminPassword();
    setCertificateFilepath(other.certificateFilepath);
    setPrivateKeyFilepath(other.privateKeyFilepath);
    this->certificateChain = other.certificateChain;
    this->privateKey = other.privateKey;
    this->serverMessage = other.serverMessage;
  }

  void ServerSettings::credentialChanged(QString)
  {
    if(!autoUpdateCredentials)
      return;
    if(!tryingLater)
    {
      tryingLater = true;
      QTimer::singleShot(1000,this,&ServerSettings::fillCredentialWatcher);
    }
  }

  void ServerSettings::fillCredentialWatcher()
  {
    tryingLater = false;
    bool tryLater = false;

    if(!certificateFilepath.isEmpty())
    {
      if(QFile::exists(getCertificateFilepath()))
      {
        loadCert();
        if(!credentialWatcher->files().contains(getCertificateFilepath()))
          credentialWatcher->addPath(getCertificateFilepath());
      }
      else
        tryLater = true;
    }

    if(!privateKeyFilepath.isEmpty())
    {
      if(QFile::exists(getPrivateKeyFilepath()))
      {
        loadKey();
        if(!credentialWatcher->files().contains(getPrivateKeyFilepath()))
          credentialWatcher->addPath(getPrivateKeyFilepath());
      }
      else
        tryLater = true;
    }

    if(tryLater && !tryingLater)
    {
      tryingLater = true;
      //QtIO::err << "Certificate or Private Key File missing. Trying later." << endl;
      QTimer::singleShot(60000,this,&ServerSettings::fillCredentialWatcher);
    }
  }
}

