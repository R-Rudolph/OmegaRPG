#ifndef GENERICROOMVARIABLE_H
#define GENERICROOMVARIABLE_H

#include "roomvariable.h"

class GenericRoomVariable : public RoomVariable
{
  QJsonValue _data;
  QString _name;
public:
  GenericRoomVariable(const QString& variableName);
  GenericRoomVariable(const QString& variableName, const QJsonValue& data);
public:
  QJsonValue data() const;
  virtual const QString& name() const;
};

#endif // GENERICROOMVARIABLE_H
