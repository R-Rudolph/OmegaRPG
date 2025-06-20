#ifndef MAPMINIATURERESOURCE_H
#define MAPMINIATURERESOURCE_H

#include "../resource.h"
#include<QString>
#include<QPointF>
#include<QJsonObject>

class MapMiniatureResource : public Resource
{
public:
  static const QString resourceName;
private:
public:
  enum Layer
  {
    LAYER_ABOVELINES,
    LAYER_BELOWLINES,
    LAYER_BACKGROUND,
    LAYER_ABOVEFOG
  };
  enum Display
  {
    DISPLAY_NORMAL,
    DISPLAY_CIRCLED,
    DISPLAY_SQUARE
  };
  enum Visibility
  {
    VISIBILITY_VISIBLE,
    VISIBILITY_PARTIAL,
    VISIBILITY_INVISIBLE
  };
private:
  QString _name;
  QVector<QString> graphic;
  quint32 selectedGraphicsIndex;
  QPointF pos;
  qreal size;
  Layer layer;
  Display display;
  qreal rotation, direction;
  Visibility visibility;
  void updateValidity();
public:
  MapMiniatureResource(const QString& name="",
                       const QVector<QString>& graphic=QVector<QString>(),
                       const QPointF& pos=QPointF(0,0),
                       qreal size=1.0,
                       MapMiniatureResource::Layer layer=LAYER_ABOVELINES,
                       MapMiniatureResource::Display display=DISPLAY_NORMAL,
                       qreal rotation=0.0,
                       qreal direction=-1.0,
                       Visibility visibility = VISIBILITY_VISIBLE,
                       quint32 selectedGraphicsIndex=0);
  MapMiniatureResource(const QJsonObject& data);
  virtual QJsonObject data() const;
  QString getName() const;
  void setName(const QString &value);
  QVector<QString> getGraphic() const;
  void setGraphic(const QVector<QString> &value);
  QPointF getPos() const;
  void setPos(const QPointF &value);
  qreal getSize() const;
  void setSize(const qreal &value);
  Layer getLayer() const;
  void setLayer(const Layer &value);
  Display getDisplay() const;
  void setDisplay(const Display &value);
  qreal getRotation() const;
  void setRotation(const qreal &value);
  qreal getDirection() const;
  void setDirection(const qreal &value);
  Visibility getVisibility() const;
  void setVisibility(const Visibility &value);
  quint32 getSelectedGraphicsIndex() const;
  void setSelectedGraphicsIndex(quint32 value);
  QString getSelectedGraphic() const;
  virtual const QString& name() const;
};


#endif // MAPMINIATURERESOURCE_H
