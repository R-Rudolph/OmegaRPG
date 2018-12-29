#include "logstatuselement.h"

const QString LogStatusElement::statusString = "status";
const QString LogStatusElement::typeString = "status";

const QMap<LogStatusElement::StatusType,QString> LogStatusElement::statusStringMap = QMap<LogStatusElement::StatusType,QString>({
    std::pair<LogStatusElement::StatusType,QString>(LogStatusElement::Undefined,"Undefined Status."),
    std::pair<LogStatusElement::StatusType,QString>(LogStatusElement::Disconnect,"Disconnected from server."),
    std::pair<LogStatusElement::StatusType,QString>(LogStatusElement::LeftRoom,"Left the room."),
    std::pair<LogStatusElement::StatusType,QString>(LogStatusElement::Quit,"Quit the program."),
    std::pair<LogStatusElement::StatusType,QString>(LogStatusElement::JoinedRoom,"Joined the room.")
    });

LogStatusElement::StatusType LogStatusElement::getStatus() const
{
  return status;
}

QString LogStatusElement::getStatusString() const
{
  return statusStringMap[status];
}

BaseLogElement* LogStatusElement::copy() const
{
  return new LogStatusElement(*this);
}

LogStatusElement::LogStatusElement(const QJsonObject& object) : AbstractLogElement(object)
{
  if(type(object)==getType())
  {
    valid = true;
    this->status = (StatusType) object[statusString].toInt(0);
  }
}

LogStatusElement::LogStatusElement(LogStatusElement::StatusType status, const QDateTime& time) : AbstractLogElement(time)
{
  this->valid = true;
  this->status = status;
}

QString LogStatusElement::getType() const
{
  return typeString;
}

QJsonObject LogStatusElement::toJson() const
{
  QJsonObject object = AbstractLogElement::toJson();
  object.insert(statusString,(int)status);
  return object;
}
