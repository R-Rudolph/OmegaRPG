#ifndef MAPFOGRESOURCE_H
#define MAPFOGRESOURCE_H

#include "../resource.h"
#include <QPointF>
#include <QJsonObject>

struct PointWrapper
{
  QPointF point;
  PointWrapper(const QPointF& point);
  bool operator<(const PointWrapper& other) const;
};

class MapFogResource : public Resource
{
public:
  static const QString resourceName;
private:
  QPointF center;
public:
  MapFogResource(const QPointF& center=QPointF(0,0));
  MapFogResource(const QJsonObject& data);
  virtual QJsonObject data() const;
  QPointF getCenter() const;
  void setCenter(const QPointF &value);
  virtual const QString& name() const;
};

#endif // MAPFOGRESOURCE_H
