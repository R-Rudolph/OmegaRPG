#include "serverentry.h"

ServerEntry::ServerEntry(QString address, QString name)
{
  this->address = address;
  this->name = name;
  players = 0;
}

ServerEntry::ServerEntry(const QJsonObject &obj)
{
  this->address = obj["addr"].toString();
  this->name = obj["name"].toString();
  players = 0;
}

QJsonObject ServerEntry::toJson() const
{
  QJsonObject obj;
  obj.insert("name",name);
  obj.insert("addr",address);
  return obj;
}
