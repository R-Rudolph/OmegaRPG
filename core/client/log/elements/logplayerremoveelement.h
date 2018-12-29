#ifndef LOGPLAYERREMOVEELEMENT_H
#define LOGPLAYERREMOVEELEMENT_H

#include "core/client/log/elements/abstractlogelement.h"

class LogPlayerRemoveElement : public AbstractLogElement
{
  static const QString idString;
  QString id;
public:
  static const QString typeString;
  LogPlayerRemoveElement(const QString& id, const QDateTime& time);
  LogPlayerRemoveElement(const QJsonObject& object);
  virtual QString getType() const;
  virtual QJsonObject toJson() const;

  QString getId() const;
  virtual BaseLogElement* copy() const;
};

#endif // LOGPLAYERREMOVEELEMENT_H
