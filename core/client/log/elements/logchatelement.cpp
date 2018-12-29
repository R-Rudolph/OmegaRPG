#include "logchatelement.h"

const QString LogChatElement::fromString = "from";
const QString LogChatElement::toString = "to";
const QString LogChatElement::nameString = "name";
const QString LogChatElement::colorString = "color";
const QString LogChatElement::textString = "text";
const QString LogChatElement::typeString = "chat";

QString LogChatElement::getFrom() const
{
  return from;
}

QString LogChatElement::getTo() const
{
  return to;
}

QString LogChatElement::getName() const
{
  return name;
}

QString LogChatElement::getColor() const
{
  return color;
}

QString LogChatElement::getText() const
{
  return text;
}

BaseLogElement* LogChatElement::copy() const
{
  return new LogChatElement(*this);
}

QJsonObject LogChatElement::toJson() const
{
  QJsonObject object = AbstractLogElement::toJson();
  object.insert(fromString,getFrom());
  object.insert(toString,getTo());
  object.insert(nameString,getName());
  object.insert(colorString,getColor());
  object.insert(textString,getText());
  return object;
}

QString LogChatElement::getType() const
{
  return typeString;
}

LogChatElement::LogChatElement(const QJsonObject& object) : AbstractLogElement(object)
{
  if(type(object)==getType())
  {
    this->valid = true;
    this->from   = object[fromString].toString();
    this->to     = object[toString].toString();
    this->name   = object[nameString].toString();
    this->color  = object[colorString].toString();
    this->text   = object[textString].toString();
  }
}

LogChatElement::LogChatElement(const QString& from,
                               const QString& to,
                               const QString& name,
                               const QString& color,
                               const QString& text,
                               const QDateTime& time) : AbstractLogElement(time)
{
  this->valid = true;
  this->from = from;
  this->to = to;
  this->name = name;
  this->color = color;
  this->text = text;
}
