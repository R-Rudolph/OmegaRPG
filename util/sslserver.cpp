#include "sslserver.h"

SslServer::SslServer(QObject *parent) : QTcpServer(parent)
{

}

QSslSocket *SslServer::nextPendingSslConnection()
{
  QSslSocket* socket = (QSslSocket*) nextPendingConnection();
  disconnect(socket,&QSslSocket::encrypted,this,&SslServer::sslConnectionReady);
  return socket;
}

void SslServer::incomingConnection(qintptr socketDescriptor)
{
  QSslSocket *serverSocket = new QSslSocket();
  serverSocket->setLocalCertificateChain(certificateChain_);
  serverSocket->setPrivateKey(privateKey_);
  if(serverSocket->setSocketDescriptor(socketDescriptor))
  {
    connect(serverSocket, &QSslSocket::encrypted, this, &SslServer::socketEncrypted);
    connect(serverSocket, &QSslSocket::disconnected, this, &SslServer::socketDisconnected);
    //connect(serverSocket, &QSslSocket::readyRead, this, &SslServer::socketDisconnected);
    serverSocket->startServerEncryption();
  }
  else
  {
    delete serverSocket;
  }
}

void SslServer::setCertificateChain(const QList<QSslCertificate> &cert)
{
  certificateChain_ = cert;
}

void SslServer::setPrivateKey(const QSslKey &key)
{
  privateKey_ = key;
}

void SslServer::sslErrors(const QList<QSslError>&)
{

}

void SslServer::socketEncrypted()
{
  QSslSocket* socket = (QSslSocket*) QObject::sender();
  disconnect(socket, &QSslSocket::encrypted, this, &SslServer::socketEncrypted);
  disconnect(socket, &QSslSocket::disconnected, this, &SslServer::socketDisconnected);
  //disconnect(socket, &QSslSocket::readyRead, this, &SslServer::socketDisconnected);
  addPendingConnection(socket);
  emit sslConnectionReady();
}

void SslServer::socketDisconnected()
{
  QSslSocket* socket = (QSslSocket*) QObject::sender();
  disconnect(socket, &QSslSocket::encrypted, this, &SslServer::socketEncrypted);
  disconnect(socket, &QSslSocket::disconnected, this, &SslServer::socketDisconnected);
  //disconnect(socket, &QSslSocket::readyRead, this, &SslServer::socketDisconnected);
  socket->close();
  socket->deleteLater();
}
