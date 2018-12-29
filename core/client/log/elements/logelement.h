#ifndef LOGELEMENT_H
#define LOGELEMENT_H

#include "core/client/log/elements/abstractlogelement.h"

class LogChatElement;
class LogInfoElement;
class LogPlayerElement;
class LogPlayerRemoveElement;
class LogStatusElement;

class LogElement : public BaseLogElement
{
public:
  enum ElementType
  {
    UnknownType,
    Chat,
    Info,
    Player,
    PlayerRemove,
    Status
  };
public:
  static const QMap<QString,ElementType>& typeMap();
private:
  static QMap<QString,ElementType> _typeMap;
  ElementType elementType;
  BaseLogElement* element;
public:
  LogElement();
  LogElement(const LogElement& other);
  LogElement(const QJsonObject& object);
  ~LogElement();
  LogElement(const BaseLogElement& element);

  virtual QJsonObject toJson() const;
  virtual QString getType() const;
  virtual QDateTime getTime() const;

  bool isChatElement() const;
  bool isInfoElement() const;
  bool isPlayerElement() const;
  bool isPlayerRemoveElement() const;
  bool isStatusElement() const;

  const LogChatElement*         chatElement() const;
  const LogInfoElement*         infoElement() const;
  const LogPlayerElement*       playerElement() const;
  const LogPlayerRemoveElement* playerRemoveElement() const;
  const LogStatusElement*       statusElement() const;

  ElementType getElementType() const;
  virtual BaseLogElement* copy() const;
  virtual bool isValid() const;
};

#endif // LOGELEMENT_H
