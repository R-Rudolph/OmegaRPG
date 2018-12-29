#ifndef METASERVERCLIENT_H
#define METASERVERCLIENT_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>

#include <QMap>
#include "core/multi/util/serverlist.h"

class MetaServerClient : public QObject
{
  Q_OBJECT
  QStringList httpMetaServerUrls;
  QNetworkAccessManager* notifyManager;
  QNetworkAccessManager* getManager;
  ServerList serverList;
public:
  enum MetaServerStatus
  {
    STATUS_NONE,
    STATUS_OK,
    STATUS_CONNECTION_ERROR,
    STATUS_PARSE_ERROR
  };
  static QString metaServerStatusString(MetaServerStatus status);
  explicit MetaServerClient(QObject *parent = 0);
private:
  QMap<QString, MetaServerStatus> statusMap;
signals:
  void notifySuccessful();
  void getSuccessful(ServerList);
  void notifyFailed(QNetworkReply::NetworkError error);
  void getFailed(QNetworkReply::NetworkError error, QString errorMsg);
  void statusChanged(QMap<QString, MetaServerStatus>);
public slots:
  void notifyHttpMetaServer(const QString& serverName, const QString& serverUrl, int port, int players);
  void getServersHttpMetaServer();
  void setHttpMetaServerUrls(const QStringList &values);
private slots:
  void httpNofityFinished(QNetworkReply* reply);
  void httpGetFinished(QNetworkReply* reply);
};

#endif // METASERVERCLIENT_H
