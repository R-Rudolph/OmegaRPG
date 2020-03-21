#include "networkclient.h"

namespace orpg
{

  /*
   *  UTILITY FUNCTIONS
   */

  quint32 NetworkClient::endianSwitch(quint32 value)
  {
    quint32 test = 1;
    char* ptr=(char*)&test;
    if(ptr[0])
    {
      char temp;
      ptr = (char*)&value;
      temp = ptr[0];
      ptr[0]=ptr[3];
      ptr[3]=temp;
      temp = ptr[2];
      ptr[2]=ptr[1];
      ptr[1]=temp;
    }
    return value;
  }

  quint32 NetworkClient::binInt(const QByteArray& num)
  {
    if(num.length()<4)
      return 0;
    quint32 result;
    char* ptr = (char*) &result;
    for(int i=0;i<4;i++)
      ptr[i]=num[i];
    result = endianSwitch(result);
    return result;
  }

  QByteArray NetworkClient::binInt(quint32 num)
  {
    num = endianSwitch(num);
    char* ptr = (char*) &num;
    QByteArray arr;
    arr.resize(4);
    for(int i=0;i<4;i++)
      arr[i]=ptr[i];
    return arr;
  }

  void NetworkClient::setSecurity(QSslSocket& socket)
  {
    socket.setProtocol(QSsl::TlsV1_2);
    QSslConfiguration conf = socket.sslConfiguration();
    QList<QSslCipher> ciphers;
    foreach(const QSslCipher& cipher, QSslConfiguration::supportedCiphers())
    {
      if(cipher.keyExchangeMethod()=="DH" || cipher.keyExchangeMethod()=="ECDH")
        ciphers.append(cipher);
    }
    conf.setCiphers(ciphers);
    socket.setSslConfiguration(conf);
  }

  const QByteArray NetworkClient::greetingMessage = QString("OMEGARPG").toUtf8();

  NetworkClient::NetworkClient(QObject *parent) : QObject(parent)
  {
    socket = new QSslSocket(this);
    setSecurity(*socket);
    socket->setPeerVerifyMode(QSslSocket::VerifyPeer);
    blocksize = 0;
    isConnected = 0;
    QObject::connect(socket, &QIODevice::readyRead, this, &NetworkClient::readMessage);
    QObject::connect(socket, &QAbstractSocket::connected, this, &NetworkClient::connected);
    QObject::connect(socket, &QSslSocket::encrypted, this, &NetworkClient::socketConnected);
    QObject::connect(socket, &QAbstractSocket::disconnected, this, &NetworkClient::socketDisconnected);
    QObject::connect(socket, qOverload<QAbstractSocket::SocketError>(&QAbstractSocket::error), this, &NetworkClient::socketError);
    QObject::connect(socket, qOverload<const QList<QSslError>&>(&QSslSocket::sslErrors),this,&NetworkClient::sslError);
    QObject::connect(SslSettings::get(),&SslSettings::exceptionsChanged,this,&NetworkClient::exceptionsChanged);
    exceptionsChanged();
  }

  NetworkClient::NetworkClient(QSslSocket *socket, QObject *parent) : QObject(parent)
  {
    this->socket = socket;
    setSecurity(*socket);
    socket->setParent(this);
    socket->setPeerVerifyMode(QSslSocket::VerifyNone);
    socket->write(NetworkClient::greetingMessage);
    socket->waitForBytesWritten(50);
    blocksize = greetingMessage.size();
    greetingDone = false;
    isConnected = 1;
    QObject::connect(socket, &QIODevice::readyRead, this, &NetworkClient::readMessage);
    QObject::connect(socket, &QAbstractSocket::connected, this, &NetworkClient::connected);
    QObject::connect(socket, &QSslSocket::encrypted, this, &NetworkClient::socketConnected);
    QObject::connect(socket, &QAbstractSocket::disconnected, this, &NetworkClient::socketDisconnected);
    QObject::connect(socket, qOverload<QAbstractSocket::SocketError>(&QAbstractSocket::error), this, &NetworkClient::socketError);
    QObject::connect(socket, qOverload<const QList<QSslError>&>(&QSslSocket::sslErrors),this,&NetworkClient::sslError);
  }

  NetworkClient::~NetworkClient()
  {
    delete socket;
  }

  void NetworkClient::sendMessage(const orpg::Message &message)
  {
    if(!message.isValid())
      return;
    if(!isConnected)
        return;
    QByteArray arr = message.toByteArray();
    qint64 sent = socket->write(binInt(arr.size()));
    if(sent==-1)
    {
      emit sendingError();
      return;
    }
    sent = socket->write(arr);
    if(sent==-1)
    {
      emit sendingError();
      return;
    }
  }

  void NetworkClient::connect(QString address, QString port)
  {
    blocksize = NetworkClient::greetingMessage.size();
    greetingDone = 0;
    socket->abort();
    socket->connectToHostEncrypted(address, port.toInt());
  }

  void NetworkClient::disconnect()
  {
    if(isConnected)
    {
      isConnected = 0;
    }
    socket->disconnectFromHost();
  }

  void NetworkClient::readMessage()
  {
    while(socket->isEncrypted())
    {
      if(blocksize==0)
      {
         if(socket->bytesAvailable()<4)
           break;
         QByteArray length = socket->read(4);
         blocksize = binInt(length);
      }
      else
      {
        if(socket->bytesAvailable()<blocksize)
          break;
        QByteArray data = socket->read(blocksize);
        if(greetingDone)
        {
          blocksize = 0;
          Message message(data);
          emit receivedMessage(message);
        }
        else
        {
          if(data != greetingMessage)
          {
            emit incorrectPeer();
            disconnect();
            break;
          }
          else
          {
            greetingDone = true;
            blocksize = 0;
            emit correctPeer();
          }
        }
      }
    }
  }

  void NetworkClient::socketConnected()
  {

    isConnected = 1;
    blocksize = NetworkClient::greetingMessage.size();
    if(socket->write(NetworkClient::greetingMessage)==-1)
    {
      emit sendingError();
      return;
    }
    emit encrypted();
  }

  void NetworkClient::socketDisconnected()
  {
    isConnected = 0;
    emit disconnected();
  }

  void NetworkClient::socketError(QAbstractSocket::SocketError err)
  {
     isConnected = 0;
     socket->disconnectFromHost();
     socket->abort();
     emit error(err);
  }

  void NetworkClient::sslError(const QList<QSslError>& errors)
  {
    QList<QSslError> newErrors;
    foreach (QSslError error, errors)
    {
      if(!SslSettings::get()->isInExceptions(error))
        newErrors.append(error);
    }
    if(newErrors.size()>0)
      emit sslErrorsSignal(newErrors);
  }

  void NetworkClient::exceptionsChanged()
  {
    socket->ignoreSslErrors(SslSettings::get()->allExceptions().values());
  }
}
