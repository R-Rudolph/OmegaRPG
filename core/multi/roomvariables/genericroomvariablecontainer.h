#ifndef GENERICROOMVARIABLECONTAINER_H
#define GENERICROOMVARIABLECONTAINER_H

#include <QObject>
#include "roomvariablecontainer.h"

class GenericRoomVariableContainer : public RoomVariableContainer
{
  Q_OBJECT
  QString _name;
public:
  explicit GenericRoomVariableContainer(const QString& name,QObject *parent = nullptr);
  // RoomVariableContainer interface
protected:
  RoomVariable *newResource();
  RoomVariable *newResource(const QString& resourceName, const QJsonValue &data);
public:
  const QString &name() const;
};

#endif // GENERICROOMVARIABLECONTAINER_H
