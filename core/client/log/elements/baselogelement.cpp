#include "baselogelement.h"

const QString BaseLogElement::timeString = "time";
const QString BaseLogElement::typeString = "type";


QString BaseLogElement::type(const QJsonObject& object) const
{
  return object[typeString].toString();
}

BaseLogElement::BaseLogElement()
{

}

BaseLogElement::~BaseLogElement()
{

}
