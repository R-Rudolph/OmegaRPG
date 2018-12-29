#include "alias.h"

QString Alias::getName() const
{
  return name;
}

void Alias::setName(const QString &value)
{
  name = value;
}

QString Alias::getColor() const
{
  return color;
}

void Alias::setColor(const QString &value)
{
  color = value;
}

bool Alias::getSelected() const
{
  return selected;
}

void Alias::setSelected(bool value)
{
  selected = value;
}

AliasSheetBinder *Alias::getBinder() const
{
  return binder;
}

void Alias::setBinder(AliasSheetBinder *value)
{
  delete binder;
  binder = value;
}

Alias::Alias(const QString &name, const QString &color, const bool& selected)
{
  this->selected = selected;
  this->name = QString(name);
  this->color = QString(color);
  this->binder = new AliasSheetBinder();
}

Alias::Alias(const QJsonObject &json) : Alias()
{
  bool s = false;
  if(json.contains("name"))
    this->name = json["name"].toString();
  if(json.contains("color"))
    this->color = json["color"].toString();
  if(json.contains("selected"))
    s = json["selected"].toBool();
  selected = s;
  this->binder = new AliasSheetBinder(json["binder"].toObject());
}

Alias::Alias(const Alias &other)
{
  this->name = other.name;
  this->color = other.color;
  this->selected = other.selected;
  this->binder = (AliasSheetBinder*) other.binder->copy();
}

Alias::~Alias()
{
  delete binder;
}

QJsonObject Alias::toJson() const
{
  QJsonObject json;
  json.insert("color",color);
  json.insert("name",name);
  json.insert("selected",QJsonValue(selected));
  json.insert("binder",binder->toJson());
  return json;
}

QVariant Alias::toVariant()
{
  return QVariant(QStringList({name,color}));
}

bool Alias::operator ==(const Alias &other)
{
  return (other.name==name) && (other.color==color);
}

Alias &Alias::operator =(const Alias &other)
{
  this->name = other.name;
  this->color = other.color;
  this->selected = other.selected;
  this->binder = (AliasSheetBinder*) other.binder->copy();
  return *this;
}
