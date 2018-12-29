#include "logelement.h"

#include "logchatelement.h"
#include "loginfoelement.h"
#include "logplayerelement.h"
#include "logplayerremoveelement.h"
#include "logstatuselement.h"

QMap<QString,LogElement::ElementType> LogElement::_typeMap;

LogElement::ElementType LogElement::getElementType() const
{
  return elementType;
}

BaseLogElement* LogElement::copy() const
{
  return element->copy();
}

bool LogElement::isValid() const
{
  if(element==nullptr)
    return false;
  else
    return element->isValid();
}

const QMap<QString, LogElement::ElementType>&LogElement::typeMap()
{
  if(_typeMap.isEmpty())
  {
    _typeMap = QMap<QString,LogElement::ElementType>({
                                                       std::pair<QString,LogElement::ElementType>(LogChatElement::typeString        ,LogElement::Chat),
                                                       std::pair<QString,LogElement::ElementType>(LogInfoElement::typeString        ,LogElement::Info),
                                                       std::pair<QString,LogElement::ElementType>(LogPlayerElement::typeString      ,LogElement::Player),
                                                       std::pair<QString,LogElement::ElementType>(LogPlayerRemoveElement::typeString,LogElement::PlayerRemove),
                                                       std::pair<QString,LogElement::ElementType>(LogStatusElement::typeString      ,LogElement::Status),
                                                       });
  }
  return _typeMap;
}

LogElement::LogElement()
{
  elementType = UnknownType;
  element = nullptr;
}

LogElement::LogElement(const LogElement& other) : LogElement()
{
  if(other.isValid())
  {
    this->elementType = other.elementType;
    this->element = other.element->copy();
  }
}

LogElement::LogElement(const QJsonObject& object) : LogElement()
{
  QString objectTypeString = type(object);
  if(typeMap().contains(objectTypeString))
  {
    elementType = typeMap()[objectTypeString];
    switch(elementType)
    {
      case LogElement::Chat:
        element = new LogChatElement(object);
        break;
      case LogElement::Info:
        element = new LogInfoElement(object);
        break;
      case LogElement::Player:
        element = new LogPlayerElement(object);
        break;
      case LogElement::PlayerRemove:
        element = new LogPlayerRemoveElement(object);
        break;
      case LogElement::Status:
        element = new LogStatusElement(object);
        break;
      default:
        break;
    }
  }
}

LogElement::~LogElement()
{
  //delete element;
}

LogElement::LogElement(const BaseLogElement& element)
{
  this->element = element.copy();
  this->elementType = typeMap()[element.getType()];
}

QJsonObject LogElement::toJson() const
{
  if(element!=nullptr)
    return element->toJson();
  else
    return QJsonObject();
}

QString LogElement::getType() const
{
  if(element!=nullptr)
    return element->getType();
  else
    return QString();
}

QDateTime LogElement::getTime() const
{
  if(element!=nullptr)
    return element->getTime();
  else
    return QDateTime();
}

bool LogElement::isChatElement() const
{
  return elementType==LogElement::Chat;
}

bool LogElement::isInfoElement() const
{
  return elementType==LogElement::Info;
}

bool LogElement::isPlayerElement() const
{
  return elementType==LogElement::Player;
}

bool LogElement::isPlayerRemoveElement() const
{
  return elementType==LogElement::PlayerRemove;
}

bool LogElement::isStatusElement() const
{
  return elementType==LogElement::Status;
}

const LogChatElement* LogElement::chatElement() const
{
  if(isChatElement())
    return static_cast<LogChatElement*>(element);
  else
    return nullptr;
}

const LogInfoElement* LogElement::infoElement() const
{
  if(isInfoElement())
    return static_cast<LogInfoElement*>(element);
  else
    return nullptr;
}

const LogPlayerElement* LogElement::playerElement() const
{
  if(isPlayerElement())
    return static_cast<LogPlayerElement*>(element);
  else
    return nullptr;
}

const LogPlayerRemoveElement* LogElement::playerRemoveElement() const
{
  if(isPlayerRemoveElement())
    return static_cast<LogPlayerRemoveElement*>(element);
  else
    return nullptr;
}

const LogStatusElement* LogElement::statusElement() const
{
  if(isStatusElement())
    return static_cast<LogStatusElement*>(element);
  else
    return nullptr;
}
