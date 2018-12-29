#include "roominfo.h"

const QString RoomInfo::idField = "id";
const QString RoomInfo::nameField = "name";
const QString RoomInfo::hasPasswordField = "password";
const QString RoomInfo::playerNumberField = "pnum";
const QString RoomInfo::timeField = "time";
const QString RoomInfo::timeFormat = "yyyy-MM-dd hh-mm-ss";

int RoomInfo::id() const
{
  return _id;
}

void RoomInfo::setId(int id)
{
  _valid = true;
  _id = id;
}

QString RoomInfo::name() const
{
  return _name;
}

void RoomInfo::setName(const QString& name)
{
  _name = name;
}

int RoomInfo::numPlayers() const
{
  return _numPlayers;
}

void RoomInfo::setNumPlayers(int numPlayers)
{
  _numPlayers = numPlayers;
}

bool RoomInfo::hasPassword() const
{
  return _hasPassword;
}

void RoomInfo::setHasPassword(bool hasPassword)
{
  _hasPassword = hasPassword;
}

QString RoomInfo::uid() const
{
  return QString("%1-%2-%3").arg(_time.toString(Qt::ISODate),QString::number(_id),_name);
}

QDateTime RoomInfo::time() const
{
  return _time;
}

RoomInfo::RoomInfo(const QString &name, bool hasPassword, int id, int numPlayers, QDateTime time)
{
  _valid = true;
  _id = id;
  _name = name;
  _numPlayers = numPlayers;
  _hasPassword = hasPassword;
  _time = time;
}

RoomInfo::RoomInfo(const QJsonObject &object)
{
  _id = object[idField].toInt(-1);
  _valid = _id != -1;
  if(!_valid)
  {
    _id = object[idField].toString().toInt(&_valid);
  }
  _name = object[nameField].toString();
  _hasPassword = object[hasPasswordField].toBool(false);
  _numPlayers = object[playerNumberField].toInt(0);
  _time = QDateTime::fromString(object[timeField].toString(),Qt::ISODate);
}

RoomInfo::RoomInfo()
{
  _valid = false;
  _id = -1;
  _numPlayers = 0;
  _hasPassword = false;
}

QJsonObject RoomInfo::toJson() const
{
  QJsonObject object;
  object.insert(idField,QString::number(_id));
  object.insert(nameField,_name);
  object.insert(hasPasswordField,_hasPassword);
  object.insert(playerNumberField,_numPlayers);
  object.insert(timeField,_time.toString(Qt::ISODate));
  return object;
}

bool RoomInfo::isValid() const
{
  return _valid;
}
