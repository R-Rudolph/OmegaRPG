#ifndef SSLSERVER_H
#define SSLSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QSslSocket>
#include <QSslKey>
#include <QSslCertificate>

class SslServer : public QTcpServer
{
  Q_OBJECT
  QSslKey privateKey_;
  QList<QSslCertificate> certificateChain_;
public:
  explicit SslServer(QObject *parent = 0);
  QSslSocket* nextPendingSslConnection();
protected:
  virtual void incomingConnection(qintptr socketDescriptor);
signals:
  void sslConnectionReady();
public slots:
  void setCertificateChain(const QList<QSslCertificate>& cert);
  void setPrivateKey(const QSslKey& key);
  void sslErrors(const QList<QSslError> &errors);
private slots:
  void socketEncrypted();
  void socketDisconnected();
};

#endif // SSLSERVER_H
