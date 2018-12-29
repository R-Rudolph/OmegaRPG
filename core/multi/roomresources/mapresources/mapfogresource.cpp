#include "mapfogresource.h"

const QString MapFogResource::resourceName = "map-fog";

QPointF MapFogResource::getCenter() const
{
  return center;
}

void MapFogResource::setCenter(const QPointF &value)
{
  center = value;
}

const QString&MapFogResource::name() const
{
  return resourceName;
}

MapFogResource::MapFogResource(const QPointF &center)
  : Resource()
{
  valid = true;
  this->center = center;
}

MapFogResource::MapFogResource(const QJsonObject &data)
  : Resource()
{
  valid = true;
  center.setX(data["x"].toDouble());
  center.setY(data["y"].toDouble());
}

QJsonObject MapFogResource::data() const
{
  QJsonObject data;
  data.insert("x",center.x());
  data.insert("y",center.y());
  return data;
}


PointWrapper::PointWrapper(const QPointF &point)
{
  this->point = point;
}

bool PointWrapper::operator<(const PointWrapper &other) const
{
  if(point.y()==other.point.y())
    return point.x() < other.point.x();
  else
    return point.y() <other.point.y();
}
