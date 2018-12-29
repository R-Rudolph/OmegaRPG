#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <QObject>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QMap>
#include <QString>
#include <QNetworkReply>

#include "core/multi/roomresources/mapresources/mapminiatureresource.h"
#include "objects/graphicscircledeffect.h"
#include "core/client/settings.h"

class ExternalPixmapItemHandler : public QObject
{
  Q_OBJECT
  QMap<QGraphicsEllipseItem*,QString> colorItems;
  QMap<QUrl, QSet<QGraphicsPixmapItem*> > itemMap;
  QMap<QGraphicsPixmapItem*, QUrl> urlMap;
  QMap<QGraphicsPixmapItem*, int> sizeMap;
  QColor frameColor;

  QPixmap pixmapFromColor(const QColor& color, int size);
  void setPixmap(QGraphicsPixmapItem* item, const QPixmap& pixmap);
  void setPixmaps(const QUrl& url, const QPixmap& pixmap);
public:
  ExternalPixmapItemHandler(QObject* parent=Q_NULLPTR);
  QGraphicsItem* getGraphicsItem(const QString& url, int size, MapMiniatureResource::Display displayEffect);
  void removeGraphicsItem(QGraphicsItem* item);
  void reload(const QUrl& url);
  void setFrameColor(const QColor& color);
signals:
private slots:
  void downloadComplete(QUrl url);
  void downloadFailed(QUrl url, QString error);
};

#endif // IMAGEHANDLER_H
