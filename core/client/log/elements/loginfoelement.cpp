#include "loginfoelement.h"

const QString LogInfoElement::idString = "id";
const QString LogInfoElement::roomString = "room";
const QString LogInfoElement::serverString = "server";
const QString LogInfoElement::typeString = "info";

QString LogInfoElement::getServerName() const
{
  return serverName;
}

QString LogInfoElement::getViewerID() const
{
  return viewerID;
}

BaseLogElement* LogInfoElement::copy() const
{
  return new LogInfoElement(*this);
}

RoomInfo LogInfoElement::getRoomInfo() const
{
  return roomInfo;
}

LogInfoElement::LogInfoElement() : AbstractLogElement()
{

}

LogInfoElement::LogInfoElement(const QJsonObject& object) : AbstractLogElement(object)
{
  if(type(object)==getType())
  {
    valid = true;
    this->viewerID = object[idString].toString();
    this->roomInfo = RoomInfo(object[roomString].toObject());
    this->serverName = object[serverString].toString();
  }
}

LogInfoElement::LogInfoElement(const QString& serverName, const RoomInfo& roomInfo, const QString& viewerID, const QDateTime& time) : AbstractLogElement(time)
{
  this->valid = true;
  this->viewerID = viewerID;
  this->serverName = serverName;
  this->roomInfo = roomInfo;
}

QJsonObject LogInfoElement::toJson() const
{
  QJsonObject object = AbstractLogElement::toJson();
  object.insert(idString,getViewerID());
  object.insert(roomString,roomInfo.toJson());
  object.insert(serverString,getServerName());
  return object;
}

QString LogInfoElement::getType() const
{
  return typeString;
}
