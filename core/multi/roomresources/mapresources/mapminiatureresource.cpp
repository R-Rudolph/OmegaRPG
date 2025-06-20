#include "mapminiatureresource.h"
#include <QJsonArray>

const QString MapMiniatureResource::resourceName = "map-miniature";

QString MapMiniatureResource::getName() const
{
  return _name;
}

void MapMiniatureResource::setName(const QString &value)
{
  _name = value;
}

QVector<QString> MapMiniatureResource::getGraphic() const
{
  return graphic;
}

void MapMiniatureResource::setGraphic(const QVector<QString> &value)
{
  graphic = value;
}

QPointF MapMiniatureResource::getPos() const
{
  return pos;
}

void MapMiniatureResource::setPos(const QPointF &value)
{
  pos = value;
}

qreal MapMiniatureResource::getSize() const
{
  return size;
}

void MapMiniatureResource::setSize(const qreal &value)
{
  size = value;
}

MapMiniatureResource::Layer MapMiniatureResource::getLayer() const
{
  return layer;
}

void MapMiniatureResource::setLayer(const Layer &value)
{
  layer = value;
  updateValidity();
}

MapMiniatureResource::Display MapMiniatureResource::getDisplay() const
{
  return display;
}

void MapMiniatureResource::setDisplay(const Display &value)
{
  display = value;
  updateValidity();
}

qreal MapMiniatureResource::getRotation() const
{
  return rotation;
}

void MapMiniatureResource::setRotation(const qreal &value)
{
  rotation = value;
}

qreal MapMiniatureResource::getDirection() const
{
  return direction;
}

void MapMiniatureResource::setDirection(const qreal &value)
{
  direction = value;
}

MapMiniatureResource::Visibility MapMiniatureResource::getVisibility() const
{
  return visibility;
}

void MapMiniatureResource::setVisibility(const Visibility &value)
{
  visibility = value;
}

quint32 MapMiniatureResource::getSelectedGraphicsIndex() const {
  return selectedGraphicsIndex;
}

void MapMiniatureResource::setSelectedGraphicsIndex(quint32 value) {
  selectedGraphicsIndex = value;
}

const QString&MapMiniatureResource::name() const
{
  return resourceName;
}

void MapMiniatureResource::updateValidity()
{
  valid =
      (layer>=LAYER_ABOVELINES) &&
      (layer<=LAYER_ABOVEFOG) &&
      (display>=DISPLAY_NORMAL) &&
      (display<=DISPLAY_SQUARE) &&
      (visibility>=VISIBILITY_VISIBLE) &&
      (visibility<=VISIBILITY_INVISIBLE);
}

MapMiniatureResource::MapMiniatureResource(const QString &name,
                                           const QVector<QString> &graphic,
                                           const QPointF &pos,
                                           qreal size,
                                           MapMiniatureResource::Layer layer,
                                           MapMiniatureResource::Display display,
                                           qreal rotation,
                                           qreal direction,
                                           MapMiniatureResource::Visibility visibility,
                                           quint32 selectedGraphicsIndex)
  : Resource()
{
  this->_name = name;
  this->graphic = graphic;
  this->pos = pos;
  this->size = size;
  this->layer = layer;
  this->display = display;
  this->rotation = rotation;
  this->direction = direction;
  this->visibility = visibility;
  this->selectedGraphicsIndex = selectedGraphicsIndex;
  updateValidity();
}

MapMiniatureResource::MapMiniatureResource(const QJsonObject &data)
  : Resource()
{
  this->_name = data["name"].toString();
  QVector<QString> graphicArray;
  if (data["graphic"].isString()) {
    graphicArray = QVector<QString>({data["graphic"].toString()});
  } else if (data["graphic"].isArray()) {
      for (const QJsonValue &entry : data["graphic"].toArray()) {
          graphicArray.push_back(entry.toString());
      }
  }
  this->graphic = graphicArray;
  this->selectedGraphicsIndex = data["selectedGraphicsIndex"].toInt(0);
  this->pos.rx() = data["x"].toDouble(0.0);
  this->pos.ry() = data["y"].toDouble(0.0);
  this->size = data["size"].toDouble(1);
  this->layer = (MapMiniatureResource::Layer) data["layer"].toInt(LAYER_ABOVELINES);
  this->display = (MapMiniatureResource::Display) data["display"].toInt(DISPLAY_NORMAL);
  this->rotation = data["rotation"].toDouble(0.0);
  this->direction = data["direction"].toDouble(-1.0);
  this->visibility = (MapMiniatureResource::Visibility) data["vis"].toInt(VISIBILITY_VISIBLE);
  updateValidity();
}

QJsonObject MapMiniatureResource::data() const
{
  QJsonObject data;
  data.insert("name",_name);
  QJsonArray graphicsArray;
  for (const QString &entry : graphic) {
      graphicsArray.push_back(entry);
  }
  data.insert("graphic",graphicsArray);
  data.insert("x",pos.x());
  data.insert("y",pos.y());
  data.insert("size",size);
  data.insert("layer",(int)layer);
  data.insert("display",(int)display);
  data.insert("rotation",rotation);
  data.insert("direction",direction);
  data.insert("vis",(int)visibility);
  data.insert("selectedGraphicsIndex", (int) selectedGraphicsIndex);
  return data;
}

QString MapMiniatureResource::getSelectedGraphic() const {
    if (this->selectedGraphicsIndex < this->graphic.size()) {
        return this->graphic[selectedGraphicsIndex];
    } else {
        return "";
    }
}
