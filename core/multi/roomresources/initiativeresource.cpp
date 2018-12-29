#include "initiativeresource.h"

const QString InitiativeResource::initiativeName = "initiative";

QString InitiativeResource::getColor() const
{
  return color;
}

void InitiativeResource::setColor(const QString &value)
{
  color = value;
}

const QString& InitiativeResource::name() const
{
  return initiativeName;
}

InitiativeResource::InitiativeResource(const QString &name, const QString &status, const QString &iconUrl, bool isActive, int initiative, int position, const QString& color)
  : Resource()
{
  this->_name = name;
  this->status = status;
  this->iconUrl = iconUrl;
  this->isActive = isActive;
  this->initiative = initiative;
  this->position = position;
  this->color = color;
  valid = true;
}

InitiativeResource::InitiativeResource(const QJsonObject &data)
  : InitiativeResource(data["name"].toString(),
                             data["status"].toString(),
                             data["iconUrl"].toString(),
                             data["active"].toBool(),
                             data["initiative"].toInt(),
                             data["position"].toInt(),
                             data["color"].toString())
{

}

QJsonObject InitiativeResource::data() const
{
  QJsonObject data;
  data.insert("name",_name);
  data.insert("status",status);
  data.insert("iconUrl",iconUrl);
  data.insert("active",isActive);
  data.insert("initiative",initiative);
  data.insert("position",position);
  data.insert("color",color);
  return data;
}

QString InitiativeResource::getName() const
{
  return _name;
}

void InitiativeResource::setName(const QString &value)
{
  _name = value;
}

QString InitiativeResource::getStatus() const
{
  return status;
}

void InitiativeResource::setStatus(const QString &value)
{
  status = value;
}

QString InitiativeResource::getIconUrl() const
{
  return iconUrl;
}

void InitiativeResource::setIconUrl(const QString &value)
{
  iconUrl = value;
}

bool InitiativeResource::getIsActive() const
{
  return isActive;
}

void InitiativeResource::setIsActive(bool value)
{
  isActive = value;
}

int InitiativeResource::getInitiative() const
{
  return initiative;
}

void InitiativeResource::setInitiative(int value)
{
  initiative = value;
}

int InitiativeResource::getPosition() const
{
  return position;
}

void InitiativeResource::setPosition(int value)
{
  position = value;
}

bool InitiativeResource::operator <(const InitiativeResource &other) const
{
  return position < other.position;
}

bool InitiativeResource::initiativeGreater(const InitiativeResource &a, const InitiativeResource &b)
{
  if(a.getInitiative() != b.getInitiative())
  {
    return a.getInitiative() > b.getInitiative();
  }
  else
  {
    return a < b;
  }
}
