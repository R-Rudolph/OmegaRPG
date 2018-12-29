#ifndef LOGPLAYERELEMENT_H
#define LOGPLAYERELEMENT_H

#include "core/client/log/elements/abstractlogelement.h"

class LogPlayerElement : public AbstractLogElement
{
  static const QString idString;
  static const QString nameString;
  QString id;
  QString name;
public:
  static const QString typeString;
  virtual QString getType() const;
  LogPlayerElement(const QJsonObject& object);
  LogPlayerElement(const QString& id, const QString& name, const QDateTime& time);
  virtual QJsonObject toJson() const;

  QString getId() const;
  QString getName() const;
  virtual BaseLogElement* copy() const;
};

#endif // LOGPLAYERELEMENT_H
