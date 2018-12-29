#include "logplayerelement.h"

const QString LogPlayerElement::idString = "id";
const QString LogPlayerElement::nameString = "name";
const QString LogPlayerElement::typeString = "player";

QString LogPlayerElement::getId() const
{
  return id;
}

QString LogPlayerElement::getName() const
{
  return name;
}

BaseLogElement* LogPlayerElement::copy() const
{
  return new LogPlayerElement(*this);
}

QString LogPlayerElement::getType() const
{
  return typeString;
}

LogPlayerElement::LogPlayerElement(const QJsonObject& object) : AbstractLogElement(object)
{
  if(type(object)==getType())
  {
    valid = true;
    this->id = object[idString].toString();
    this->name = object[nameString].toString();
  }
}

LogPlayerElement::LogPlayerElement(const QString& id, const QString& name, const QDateTime& time) : AbstractLogElement(time)
{
  valid = true;
  this->id = id;
  this->name = name;
}

QJsonObject LogPlayerElement::toJson() const
{
  QJsonObject object = AbstractLogElement::toJson();
  object.insert(idString,id);
  object.insert(nameString,name);
  return object;
}
