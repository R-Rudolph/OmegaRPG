#ifndef LOGCHATELEMENT_H
#define LOGCHATELEMENT_H

#include "core/client/log/elements/abstractlogelement.h"

class LogChatElement : public AbstractLogElement
{
  static const QString fromString;
  static const QString toString;
  static const QString nameString;
  static const QString colorString;
  static const QString textString;

  QString from;
  QString to;
  QString name;
  QString color;
  QString text;
public:
  static const QString typeString;
  LogChatElement(const QJsonObject& object);
  LogChatElement(const QString& from,
                 const QString& to,
                 const QString& name,
                 const QString& color,
                 const QString& text,
                 const QDateTime& time);
  virtual QJsonObject toJson() const;
  virtual QString getType() const;

  QString getFrom() const;
  QString getTo() const;
  QString getName() const;
  QString getColor() const;
  QString getText() const;

  virtual BaseLogElement* copy() const;
};

#endif // LOGCHATELEMENT_H
