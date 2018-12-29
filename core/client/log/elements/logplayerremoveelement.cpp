#include "logplayerremoveelement.h"

const QString LogPlayerRemoveElement::idString = "id";
const QString LogPlayerRemoveElement::typeString = "player_remove";


QString LogPlayerRemoveElement::getId() const
{
  return id;
}

BaseLogElement* LogPlayerRemoveElement::copy() const
{
  return new LogPlayerRemoveElement(*this);
}

LogPlayerRemoveElement::LogPlayerRemoveElement(const QString& id, const QDateTime& time) : AbstractLogElement(time)
{
  this->valid = true;
  this->id = id;
}

LogPlayerRemoveElement::LogPlayerRemoveElement(const QJsonObject& object) : AbstractLogElement(object)
{
  if(type(object)==getType())
  {
    valid = true;
    this->id = object[idString].toString();
  }
}

QString LogPlayerRemoveElement::getType() const
{
  return typeString;
}

QJsonObject LogPlayerRemoveElement::toJson() const
{
  QJsonObject object = AbstractLogElement::toJson();
  object.insert(idString,id);
  return object;
}
