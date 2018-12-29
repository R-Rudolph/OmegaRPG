#include "abstractlogelement.h"

AbstractLogElement::AbstractLogElement()
{
  valid = false;
  baseValid = false;
}

AbstractLogElement::AbstractLogElement(const QJsonObject& object) : AbstractLogElement()
{
  this->time = QDateTime::fromString(object[timeString].toString(),Qt::ISODate);
  this->baseValid = time.isValid();
}

AbstractLogElement::AbstractLogElement(const QDateTime& time) : AbstractLogElement()
{
  this->time = time;
  this->baseValid = this->time.isValid();
}

AbstractLogElement::~AbstractLogElement()
{

}

bool AbstractLogElement::isValid() const
{
  return valid && baseValid;
}

QJsonObject AbstractLogElement::toJson() const
{
  QJsonObject object;
  object.insert(timeString,time.toString(Qt::ISODate));
  object.insert(typeString,this->getType());
  return object;
}

QDateTime AbstractLogElement::getTime() const
{
  return time;
}
