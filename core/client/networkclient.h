#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QByteArray>
#include <QTcpSocket>
#include <QSslSocket>
#include <QTimer>
#include <QSslConfiguration>
#include <QSslCipher>
#include <QTimer>
#include <QDateTime>
#include "util/sslsettings.h"

#include "core/multi/message/message.h"

namespace orpg
{

  class NetworkClient : public QObject
  {
    Q_OBJECT
    QList<QSslError> ignoredSslErrors;
    static const QByteArray greetingMessage;
    bool greetingDone;
    quint32 blocksize;
    //utility
    static quint32 endianSwitch(quint32);
    static quint32 binInt(const QByteArray& arr);
    static QByteArray binInt(quint32 num);
    void setSecurity(QSslSocket& socket);
  public:
    QSslSocket* socket;
    int isConnected;
    explicit NetworkClient(QObject *parent = 0);
    explicit NetworkClient(QSslSocket* socket, QObject *parent = 0);
    ~NetworkClient();
  signals:
    void receivedMessage(orpg::Message msg);
    void sendingError();
    void encrypted();
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError);
    void correctPeer();
    void incorrectPeer();
    void sslErrorsSignal(const QList<QSslError>& errors);
  public slots:
    void sendMessage(const orpg::Message& message);
    void connect(QString address, QString port);
    void disconnect();

    void readMessage();
    void socketConnected();
    void socketDisconnected();
    void socketError(QAbstractSocket::SocketError);
    void sslError(const QList<QSslError>& errors);
  private slots:
    void exceptionsChanged();

  };

}

#endif // NETWORKCLIENT_H
