#ifndef LOGSTATUSELEMENT_H
#define LOGSTATUSELEMENT_H

#include "core/client/log/elements/abstractlogelement.h"
#include<QMap>

class LogStatusElement : public AbstractLogElement
{
public:
  enum StatusType
  {
    Undefined,
    Disconnect,
    LeftRoom,
    Quit,
    JoinedRoom
  };
private:
  static const QMap<StatusType,QString> statusStringMap;
  static const QString statusString;
  StatusType status;
public:
  static const QString typeString;
  LogStatusElement(const QJsonObject& object);
  LogStatusElement(LogStatusElement::StatusType status, const QDateTime& time);
  virtual QString getType() const;
  virtual QJsonObject toJson() const;

  LogStatusElement::StatusType getStatus() const;
  QString getStatusString() const;
  virtual BaseLogElement* copy() const;
};

#endif // LOGSTATUSELEMENT_H
