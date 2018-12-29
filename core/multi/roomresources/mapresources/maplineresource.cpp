#include "maplineresource.h"

const QString MapLineResource::resourceName = "map-lines";

QLineF MapLineResource::getLine() const
{
  return line;
}

void MapLineResource::setLine(const QLineF &value)
{
  line = value;
}

int MapLineResource::getWidth() const
{
  return width;
}

void MapLineResource::setWidth(int value)
{
  width = value;
}

QString MapLineResource::getColor() const
{
  return color;
}

void MapLineResource::setColor(const QString &value)
{
  color = value;
}

const QString&MapLineResource::name() const
{
  return resourceName;
}

MapLineResource::MapLineResource()
  : Resource()
{
  valid = true;
  line = QLineF(0,0,0,0);
  width = 0;
}

MapLineResource::MapLineResource(const QJsonObject &data)
  : Resource()
{
  valid = true;
  line.setP1(QPointF(data["x1"].toDouble(),data["y1"].toDouble()));
  line.setP2(QPointF(data["x2"].toDouble(),data["y2"].toDouble()));
  width = data["w"].toInt(0);
  color = data["c"].toString();
}

MapLineResource::MapLineResource(const QLineF &line, int width, const QString &color)
{
  valid = true;
  this->line = line;
  this->width = width;
  this->color = color;
}

QJsonObject MapLineResource::data() const
{
  QJsonObject data;
  data.insert("x1",line.x1());
  data.insert("x2",line.x2());
  data.insert("y1",line.y1());
  data.insert("y2",line.y2());
  data.insert("w",width);
  data.insert("c",color);
  return data;
}
