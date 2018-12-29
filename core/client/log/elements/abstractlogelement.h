#ifndef ABSTRACTLOGELEMENT_H
#define ABSTRACTLOGELEMENT_H

#include "baselogelement.h"

class AbstractLogElement : public BaseLogElement
{
protected:
  QDateTime time;
  bool valid;
  bool baseValid;
public:
  AbstractLogElement();
  AbstractLogElement(const QJsonObject& object);
  AbstractLogElement(const QDateTime& time);
  virtual ~AbstractLogElement();

  virtual bool isValid() const;
  virtual QString getType() const=0;
  virtual QJsonObject toJson() const;
  virtual QDateTime getTime() const;
  virtual BaseLogElement* copy() const=0;
};

#endif // ABSTRACTLOGELEMENT_H
