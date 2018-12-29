#ifndef SERVERSETTINGS_H
#define SERVERSETTINGS_H

#include <QObject>
#include <QString>

#include <QJsonObject>
#include <QSslKey>
#include <QSslCertificate>

class QFileSystemWatcher;

namespace orpg
{
  class ServerSettings : public QObject
  {
    Q_OBJECT
    static bool typesRegistered;
    static void registerTypes();
    QStringList staticMetaServers;
    QStringList synchedMetaServers;
    QStringList customMetaServers;
    void setStandardValues();
    bool parse(const QJsonObject& serverSetttingsObject);
    bool autoUpdateCredentials;
    QJsonObject toJson();
    QString certificateFilepath;
    QString privateKeyFilepath;

    bool tryingLater;
    QFileSystemWatcher* credentialWatcher;
  public:
    QSslKey privateKey;
    QList<QSslCertificate> certificateChain;
    enum CredentialLoadingError
    {
      EverythingOk = 0,
      KeyFileDoesNotExist = 1,
      KeyFileNotOpened = 2,
      KeyFileInvalid = 4,
      CertFileDoesNotExist = 8,
      CertFileNotOpened = 16,
      CertFileInvalid = 32
    };
    ServerSettings::CredentialLoadingError loadKey();
    ServerSettings::CredentialLoadingError loadCert();

    explicit ServerSettings(QObject *parent = 0);
    explicit ServerSettings(const ServerSettings& other, QObject *parent = 0);
    bool notifyMetaServer;
    QString serverMessage;
    QString serverName;
    QString serverUrl;
    QString adminPassword;
    int metaServerHeartbeat;
    int port;
    void setCustomMetaServers(const QString& servers);
    QStringList getMetaServers();
    QString getCustomMetaServerString();
    QString getCertificateFilepath();
    QString getPrivateKeyFilepath();
    void setCertificateFilepath(const QString& certificateFilepath);
    void setPrivateKeyFilepath(const QString& privateKeyFilepath);
    bool load(const QString& filename="");
    bool save(const QString& filename="");
    CredentialLoadingError loadCredentials();
    void set(const ServerSettings& other);
    QString getAdminPassword() const;
    void setAdminPassword(const QString& value);

  signals:
    void certificateChainChanged(QList<QSslCertificate> certificate);
    void privateKeyChanged(QSslKey privateKey);
  public slots:
  private slots:
    void credentialChanged(QString path);
    void fillCredentialWatcher();
  };

}


#endif // SERVERSETTINGS_H
