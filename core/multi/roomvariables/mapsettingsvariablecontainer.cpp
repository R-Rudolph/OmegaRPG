#include "mapsettingsvariablecontainer.h"
#include "mapsettingsvariable.h"

MapSettingsVariableContainer::MapSettingsVariableContainer(QObject *parent) : RoomVariableContainer(parent)
{
  initializeVariable();
}

const MapSettingsVariable *MapSettingsVariableContainer::getVariable() const
{
  return static_cast<const MapSettingsVariable*>(getAbstractVariable());
}

RoomVariable *MapSettingsVariableContainer::newResource()
{
  return new MapSettingsVariable();
}

RoomVariable *MapSettingsVariableContainer::newResource(const QString&, const QJsonValue &data)
{
  return new MapSettingsVariable(data);
}

const QString &MapSettingsVariableContainer::name() const
{
  return MapSettingsVariable::variableName;
}
