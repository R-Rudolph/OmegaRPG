#include "networkhost.h"

const QByteArray NetworkHost::greetingMessage = "OMEGARPG";

quint32 NetworkHost::endianSwitch(quint32 value)
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

quint32 NetworkHost::binInt(const QByteArray& num)
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

QByteArray NetworkHost::binInt(quint32 num)
{
  num = endianSwitch(num);
  char* ptr = (char*) &num;
  QByteArray arr;
  arr.reserve(4);
  for(int i=0;i<4;i++)
    arr[i]=ptr[i];
  return arr;
}

void NetworkHost::setSecurity(QSslSocket *socket)
{
  socket->setPeerVerifyMode(QSslSocket::VerifyNone);
  socket->setProtocol(QSsl::TlsV1_2);
  QSslConfiguration conf = socket->sslConfiguration();
  QList<QSslCipher> ciphers;
  foreach(const QSslCipher& cipher, QSslConfiguration::supportedCiphers())
  {
    if(cipher.keyExchangeMethod()=="DH" || cipher.keyExchangeMethod()=="ECDH")
      ciphers.append(cipher);
  }
  conf.setCiphers(ciphers);
  socket->setSslConfiguration(conf);
}

NetworkHost::NetworkHost(QSslSocket* socket, QObject* parent) : QObject(parent)
{
  this->socket = socket;
  socket->setParent(this);
  setSecurity(socket);
  socket->write(NetworkHost::greetingMessage);
  blocksize = greetingMessage.size();
  greetingDone = false;
  connect(socket,&QSslSocket::readyRead,this,&NetworkHost::readData);
  connect(socket,&QSslSocket::disconnected,this,&NetworkHost::disconnectSocket);
  //connect(socket,static_cast<void (QSslSocket::*)(QAbstractSocket::SocketError)>(&QSslSocket::error),this,&NetworkHost::disconnectSocket);
  //connect(socket,static_cast<void (QSslSocket::*)(const QList<QSslError>&)>(&QSslSocket::sslErrors),this,&NetworkHost::disconnectSocket);
}

void NetworkHost::sendMessage(orpg::Message message)
{
  if(message.isValid())
  {
    QByteArray data = message.toByteArray();
    quint32 size = data.size();
    QByteArray sizeArray = binInt(size);
    sizeArray.append(data);
    if(socket->write(sizeArray)==-1)
    {
      disconnectSocket();
    }
  }
}

void NetworkHost::readData()
{
  while(1)
  {
    if(blocksize==0)
    {
      if(socket->bytesAvailable()>=4)
      {
        QByteArray sizeArray = socket->read(4);
        blocksize = binInt(sizeArray);
      }
      else
      {
        break;
      }
    }
    else
    {
      if(socket->bytesAvailable()>=blocksize)
      {
        QByteArray data = socket->read(blocksize);
        if(greetingDone)
        {
          orpg::Message msg(data);
          emit receivedMessage(msg);
        }
        else
        {
          if(data!=NetworkHost::greetingMessage)
          {
            disconnectSocket();
            break;
          }
          greetingDone = true;
          emit greetingOk();
        }
        blocksize = 0;
      }
      else
        break;
    }
  }
}

void NetworkHost::disconnectSocket()
{
  socket->disconnectFromHost();
  emit disconnected();
}
