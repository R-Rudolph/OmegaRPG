#include "genericroomvariablecontainer.h"
#include "genericroomvariable.h"

GenericRoomVariableContainer::GenericRoomVariableContainer(const QString &name, QObject *parent) : RoomVariableContainer(parent)
{
  _name = name;
}

RoomVariable *GenericRoomVariableContainer::newResource()
{
  return new GenericRoomVariable(_name);
}

RoomVariable *GenericRoomVariableContainer::newResource(const QString& resourceName, const QJsonValue &data)
{
  return new GenericRoomVariable(resourceName,data);
}

const QString &GenericRoomVariableContainer::name() const
{
  return _name;
}
