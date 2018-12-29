#ifndef NETWORKHOST_H
#define NETWORKHOST_H

#include <QObject>
#include <QSslSocket>
#include <QtEndian>
#include <QSslConfiguration>
#include <QSslCipher>

#include "core/multi/message/message.h"

class NetworkHost : public QObject
{
  Q_OBJECT
  static const QByteArray greetingMessage;
  QSslSocket* socket;
  quint32 blocksize;
  bool greetingDone;

  quint32 endianSwitch(quint32 value);
  quint32 binInt(const QByteArray& num);
  QByteArray binInt(quint32 num);
  void setSecurity(QSslSocket* socket);
public:
  explicit NetworkHost(QSslSocket* socket, QObject *parent = 0);

signals:
  void disconnected();
  void receivedMessage(orpg::Message message);
  void greetingOk();
public slots:
  void sendMessage(orpg::Message message);
  void disconnectSocket();
private slots:
  void readData();
};

#endif // NETWORKHOST_H
