#include "imageprovider.h"

#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QNetworkReply>
#include <QIcon>
#include "core/multi/util/global.h"

ImageProvider* ImageProvider::staticImageProvider = nullptr;

QImage ImageProvider::fixImage(const QImage &image)
{
  if(image.isNull())
    return getFailureImage();
  else
    return image;
}

void ImageProvider::reload(const QUrl &url)
{
  foreach(const QUrl& urlEntry, urlMap.values())
  {
    if(url==urlEntry)
      return;
  }
  QNetworkReply* reply = networkManager->get(QNetworkRequest(url));
  urlMap.insert(reply,url);
}

ImageProvider *ImageProvider::get()
{
  if(staticImageProvider==nullptr)
    staticImageProvider = new ImageProvider();
  return staticImageProvider;
}

ImageProvider::ImageProvider(QObject *parent) : QObject(parent)
{
  networkManager = new QNetworkAccessManager(this);
  cache = new QNetworkDiskCache(this);
  cache->setCacheDirectory(Global::imageCacheFolder());
  networkManager->setCache(cache);

  failureImage.load(":icons/error.png");
  loadingImage.load(":icons/loading.png");

  connect(networkManager,&QNetworkAccessManager::finished,this,&ImageProvider::downloadFinished);
}

QImage ImageProvider::getLoadingImage()
{
  return loadingImage;
}

QImage ImageProvider::getFailureImage()
{
  return failureImage;
}

QImage ImageProvider::getImage(const QUrl &url)
{
  if(url.isEmpty())
    return QImage();
  if(dataMap.contains(url))
  {
    QImage image = QImage::fromData(dataMap[url]);
    return fixImage(image);
  }
  else if(QIODevice* cacheDevice = cache->data(url))
  {
    QImage image = fixImage(QImage::fromData(cacheDevice->readAll()));
    delete cacheDevice;
    return image;
  }
  else
  {
    reload(url);
    return getLoadingImage();
  }
}

QIcon ImageProvider::getLoadingIcon()
{
  return QIcon(getLoadingPixmap());
}

QIcon ImageProvider::getFailureIcon()
{
  return QIcon(getFailurePixmap());
}

QIcon ImageProvider::getIcon(const QUrl &url)
{
  return QIcon(getPixmap(url));
}

QPixmap ImageProvider::getLoadingPixmap()
{
  return QPixmap::fromImage(getLoadingImage());
}

QPixmap ImageProvider::getFailurePixmap()
{
  return QPixmap::fromImage(getFailureImage());
}

QPixmap ImageProvider::getPixmap(const QUrl &url)
{
  return QPixmap::fromImage(getImage(url));
}

void ImageProvider::downloadFinished(QNetworkReply *reply)
{
  if(reply->error()==QNetworkReply::NoError)
  {
    QUrl url = urlMap[reply];
    urlMap.remove(reply);
    dataMap.remove(url);
    dataMap.insert(url,reply->readAll());
    emit imageAvailable(url);
    reply->deleteLater();
  }
  else
  {
    QUrl url = urlMap[reply];
    urlMap.remove(reply);
    emit imageUnavaiable(url,reply->errorString());
    reply->deleteLater();
  }
}
