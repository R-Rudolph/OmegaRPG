#ifndef BASELOGELEMENT_H
#define BASELOGELEMENT_H

#include <QString>
#include <QJsonObject>
#include <QDateTime>

class BaseLogElement
{
protected:
  static const QString typeString;
  static const QString timeString;
  QString type(const QJsonObject& object) const;
  QJsonObject toJson(const QString& type) const;
  BaseLogElement();
public:
  virtual ~BaseLogElement();
  virtual bool isValid() const=0;
  virtual QString getType() const=0;
  virtual QJsonObject toJson() const=0;
  virtual QDateTime getTime() const=0;
  virtual BaseLogElement* copy() const=0;
};

#endif // BASELOGELEMENT_H
