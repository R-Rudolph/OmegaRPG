#include "metaserverclient.h"
#include "util/qtio.h"

void MetaServerClient::setHttpMetaServerUrls(const QStringList &values)
{
  foreach(QString value, values)
  {
    if(value.isEmpty())
      continue;
    QString temp;
    if(value.startsWith("http://",Qt::CaseInsensitive) || value.startsWith("https://",Qt::CaseInsensitive))
      temp = value;
    else
      temp = "http://"+value;
    while(temp.endsWith("/"))
      temp.chop(1);
    httpMetaServerUrls.append(temp);
  }
}

void MetaServerClient::httpNofityFinished(QNetworkReply *reply)
{
  QNetworkReply::NetworkError error = reply->error();
  if(error==QNetworkReply::NoError)
  {
    statusMap.insert(reply->url().host(),STATUS_OK);
    emit statusChanged(statusMap);
    emit notifySuccessful();
  }
  else
  {
    QtIO::err << "Notify Error:" << reply->errorString() << "\n";
    statusMap.insert(reply->url().host(),STATUS_CONNECTION_ERROR);
    emit statusChanged(statusMap);
    emit notifyFailed(error);
  }
  reply->deleteLater();
}

void MetaServerClient::httpGetFinished(QNetworkReply *reply)
{
  QNetworkReply::NetworkError error = reply->error();
  if(error==QNetworkReply::NoError)
  {
    ServerList list;
    QByteArray content = reply->readAll();
    if(list.parseXml(content))
    {
      statusMap.insert(reply->url().host(),STATUS_OK);
      emit statusChanged(statusMap);
      serverList.insert(list);
      emit getSuccessful(serverList);
    }
    else
    {
      statusMap.insert(reply->url().host(),STATUS_PARSE_ERROR);
      emit statusChanged(statusMap);
      QtIO::err << "Parse error ("<< reply->url().toString() <<"): " << content << "\n";
      emit getFailed(error,"Server reply could not be parsed.");
    }
  }
  else
  {
    statusMap.insert(reply->url().host(),STATUS_CONNECTION_ERROR);
    emit statusChanged(statusMap);
    QtIO::err << "Get Error:" << reply->errorString() << "\n";
    emit getFailed(error,reply->errorString());
  }
  reply->deleteLater();
}

void MetaServerClient::notifyHttpMetaServer(const QString &serverName, const QString &serverUrl, int port, int players)
{
  statusMap.clear();
  foreach(QString metaServerUrl, httpMetaServerUrls)
  {
    QUrl url(metaServerUrl+"/notify.php");
    statusMap.insert(url.host(),STATUS_NONE);
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("name",serverName);
    params.addQueryItem("url",serverUrl);
    params.addQueryItem("pnum",QString::number(players));
    params.addQueryItem("port",QString::number(port));
    notifyManager->post(request,params.query().toUtf8());
  }
  emit statusChanged(statusMap);
}

void MetaServerClient::getServersHttpMetaServer()
{
  statusMap.clear();
  foreach(QString metaServerUrl, httpMetaServerUrls)
  {
    QUrl url(metaServerUrl+"/get.php");
    statusMap.insert(url.host(),STATUS_NONE);
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64; rv:49.0) Gecko/20100101 Firefox/49.0");
    getManager->get(request);
  }
  emit statusChanged(statusMap);
}

QString MetaServerClient::metaServerStatusString(MetaServerClient::MetaServerStatus status)
{
  switch(status)
  {
    case STATUS_NONE:
      return "Not connected yet";
    case STATUS_OK:
      return "Ok";
    case STATUS_CONNECTION_ERROR:
      return "Connection Error";
    case STATUS_PARSE_ERROR:
      return "Invalid Server Response";
    default:
      return "Invalid Status. This should never happen.";
  }
}

MetaServerClient::MetaServerClient(QObject *parent) : QObject(parent)
{
  notifyManager = new QNetworkAccessManager(this);
  getManager = new QNetworkAccessManager(this);
  connect(notifyManager,&QNetworkAccessManager::finished,this,&MetaServerClient::httpNofityFinished);
  connect(getManager,&QNetworkAccessManager::finished,this,&MetaServerClient::httpGetFinished);
}
