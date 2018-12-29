#include "externalpixmapitemhandler.h"
#include "imageprovider.h"
#include <QGraphicsEllipseItem>

QPixmap ExternalPixmapItemHandler::pixmapFromColor(const QColor &color, int size)
{
  QPixmap pixmap(size,size);
  pixmap.fill(Qt::transparent);
  QPainter painter(&pixmap);
  painter.setBrush(Qt::black);
  painter.drawEllipse(0,0,size-1,size-1);
  painter.setBrush(color);
  int offset = size/20;
  painter.drawEllipse(offset,offset,size-1-2*offset,size-1-2*offset);
  return pixmap;
}

void ExternalPixmapItemHandler::setPixmap(QGraphicsPixmapItem *item, const QPixmap &pixmap)
{
  item->setPixmap(pixmap);
  int maxdim = std::max(pixmap.width(),pixmap.height());
  item->setOffset(-pixmap.width()/2.0,-pixmap.height()/2.0);
  item->setScale((qreal)sizeMap[item]/(qreal)maxdim);
}

void ExternalPixmapItemHandler::setPixmaps(const QUrl &url, const QPixmap &pixmap)
{
  if(!itemMap.contains(url))
    return;
  foreach(QGraphicsPixmapItem* item, itemMap[url])
  {
    setPixmap(item,pixmap);
  }
}

ExternalPixmapItemHandler::ExternalPixmapItemHandler(QObject* parent) : QObject(parent)
{
  frameColor = Qt::black;

  connect(ImageProvider::get(),&ImageProvider::imageAvailable,this,&ExternalPixmapItemHandler::downloadComplete);
  connect(ImageProvider::get(),&ImageProvider::imageUnavaiable,this, &ExternalPixmapItemHandler::downloadFailed);
}

QGraphicsItem* ExternalPixmapItemHandler::getGraphicsItem(const QString &url, int size, MapMiniatureResource::Display displayEffect)
{
  QColor color(url);
  if(color.isValid())
  {
    float border = size/10;
    QGraphicsEllipseItem* ellipseItem = new QGraphicsEllipseItem(-size/2.0+border/2,-size/2.0+border/2,size-border,size-border);
    ellipseItem->setPen(QPen(frameColor,border));
    ellipseItem->setBrush(QBrush(color));
    colorItems.insert(ellipseItem,url);
    return ellipseItem;
  }
  else
  {
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem();
    if(!itemMap.contains(url))
      itemMap.insert(url,QSet<QGraphicsPixmapItem*>());
    itemMap[url].insert(item);
    urlMap.insert(item,url);
    sizeMap.insert(item,size);
    setPixmap(item,ImageProvider::get()->getPixmap(url));
    if(displayEffect == MapMiniatureResource::DISPLAY_CIRCLED)
    {
      item->setGraphicsEffect((QGraphicsEffect*)new GraphicsCircledEffect(frameColor));
    }
    return item;
  }
}

void ExternalPixmapItemHandler::removeGraphicsItem(QGraphicsItem *item)
{
  QGraphicsPixmapItem* pixmapItem = static_cast<QGraphicsPixmapItem*>(item);
  if(!urlMap.contains(pixmapItem))
    return;
  QUrl url = urlMap[pixmapItem];
  itemMap[url].remove(pixmapItem);
  urlMap.remove(pixmapItem);
  sizeMap.remove(pixmapItem);
}

void ExternalPixmapItemHandler::reload(const QUrl &url)
{
  ImageProvider::get()->reload(url);
  setPixmaps(url,ImageProvider::get()->getLoadingPixmap());
}

void ExternalPixmapItemHandler::setFrameColor(const QColor &color)
{
  frameColor = color;
}

void ExternalPixmapItemHandler::downloadComplete(QUrl url)
{
  QPixmap pixmap = ImageProvider::get()->getPixmap(url);
  if(pixmap.isNull())
  {
    downloadFailed(url,"Target is not a valid image.");
  }
  else
  {
    qreal resolutionLimit = orpg::Settings::get()->imageResolutionLimit;
    if(resolutionLimit>0)
    {
      resolutionLimit *= 1000000;
      int resolution = pixmap.width()*pixmap.height();
      if(resolution>resolutionLimit)
      {
        double factor = qSqrt(resolutionLimit/resolution);
        pixmap = pixmap.scaled(pixmap.size()*factor,Qt::KeepAspectRatio);
      }
    }
    setPixmaps(url,pixmap);
  }
}

void ExternalPixmapItemHandler::downloadFailed(QUrl url, QString error)
{
  setPixmaps(url,ImageProvider::get()->getFailurePixmap());
  foreach(QGraphicsPixmapItem* item, itemMap[url])
  {
    item->setData(0,error);
  }
}
