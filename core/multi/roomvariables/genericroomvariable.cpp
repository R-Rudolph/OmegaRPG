#include "genericroomvariable.h"

GenericRoomVariable::GenericRoomVariable(const QString& variableName)
{
  _name = variableName;
}

GenericRoomVariable::GenericRoomVariable(const QString& variableName, const QJsonValue& data)
{
  _name = variableName;
  _data = data;
}

QJsonValue GenericRoomVariable::data() const
{
  return _data;
}

const QString&GenericRoomVariable::name() const
{
  return _name;
}
