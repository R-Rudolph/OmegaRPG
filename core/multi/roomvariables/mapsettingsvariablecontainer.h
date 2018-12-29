#ifndef MAPSETTINGSVARIABLECONTAINER_H
#define MAPSETTINGSVARIABLECONTAINER_H

#include <QObject>
#include "roomvariablecontainer.h"

class MapSettingsVariable;

class MapSettingsVariableContainer : public RoomVariableContainer
{
  Q_OBJECT
public:
  explicit MapSettingsVariableContainer(QObject *parent = nullptr);
  const MapSettingsVariable* getVariable() const;
  // RoomVariableContainer interface
protected:
  RoomVariable *newResource();
  RoomVariable *newResource(const QString& resourceName, const QJsonValue &data);
public:
  const QString &name() const;
};

#endif // MAPSETTINGSVARIABLECONTAINER_H
