#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QObject>
#include <QMap>
#include <QImage>
#include <QUrl>
#include <QByteArray>

class QNetworkAccessManager;
class QNetworkDiskCache;
class QNetworkReply;


class ImageProvider : public QObject
{
  Q_OBJECT
  static ImageProvider* staticImageProvider;
  QMap<QNetworkReply*,QUrl> urlMap;
  QMap<QUrl,QByteArray> dataMap;
  QNetworkAccessManager* networkManager;
  QNetworkDiskCache* cache;
  QImage loadingImage;
  QImage failureImage;
  QImage fixImage(const QImage& image);
public:
  static ImageProvider* get();
  explicit ImageProvider(QObject *parent = nullptr);

  void reload(const QUrl& url);

  QImage getLoadingImage();
  QImage getFailureImage();
  QImage getImage(const QUrl& url);

  QIcon getLoadingIcon();
  QIcon getFailureIcon();
  QIcon getIcon(const QUrl& url);

  QPixmap getLoadingPixmap();
  QPixmap getFailurePixmap();
  QPixmap getPixmap(const QUrl& url);
signals:
  void imageAvailable(const QUrl& url);
  void imageUnavaiable(const QUrl& url, QString error);
public slots:
  void downloadFinished(QNetworkReply *reply);
};

#endif // IMAGEPROVIDER_H
