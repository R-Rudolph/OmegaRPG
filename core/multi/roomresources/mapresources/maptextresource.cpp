#include "maptextresource.h"

const QString MapTextResource::resourceName = "map-text";

QString MapTextResource::getText() const
{
  return text;
}

void MapTextResource::setText(const QString &value)
{
  text = value;
  updateValidity();
}

int MapTextResource::getSize() const
{
  return size;
}

void MapTextResource::setSize(int value)
{
  size = value;
}

QPointF MapTextResource::getPos() const
{
  return pos;
}

void MapTextResource::setPos(const QPointF &value)
{
  pos = value;
}

QString MapTextResource::getColor() const
{
  return color;
}

void MapTextResource::setColor(const QString &value)
{
  color = value;
}

const QString&MapTextResource::name() const
{
  return resourceName;
}

void MapTextResource::updateValidity()
{
  valid = text.length()>0;
}

MapTextResource::MapTextResource(const QString &text, const QString &color, int size, const QPointF &pos)
  : Resource()
{
  this->text = text;
  this->color = color;
  this->size = size;
  this->pos = pos;
  updateValidity();
}

MapTextResource::MapTextResource(const QJsonObject &data)
  : Resource()
{
  text = data["text"].toString();
  color = data["color"].toString();
  size = data["size"].toInt();
  pos.setX(data["x"].toDouble());
  pos.setY(data["y"].toDouble());
  updateValidity();
}

QJsonObject MapTextResource::data() const
{
  QJsonObject data;
  data.insert("text",text);
  data.insert("color",color);
  data.insert("size",size);
  data.insert("x",pos.x());
  data.insert("y",pos.y());
  return data;
}
