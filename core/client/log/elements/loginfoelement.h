#ifndef LOGINFOOBJECT_H
#define LOGINFOOBJECT_H

#include <QJsonObject>
#include <QDateTime>
#include <QString>

#include "core/client/log/elements/abstractlogelement.h"
#include "core/multi/util/roominfo.h"

class LogInfoElement : public AbstractLogElement
{
  static const QString idString;
  static const QString roomString;
  static const QString serverString;

  QString serverName;
  QString viewerID;
  RoomInfo roomInfo;
public:
  static const QString typeString;
  LogInfoElement();
  LogInfoElement(const QJsonObject& object);
  LogInfoElement(const QString& serverName, const RoomInfo& roomInfo, const QString& viewerID, const QDateTime& time);
  virtual QJsonObject toJson() const;
  virtual QString getType() const;

  QString getServerName() const;
  QString getViewerID() const;
  virtual BaseLogElement* copy() const;
  RoomInfo getRoomInfo() const;
};

#endif // LOGINFOOBJECT_H
