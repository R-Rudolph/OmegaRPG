#ifndef ROOMVARIABLECONTAINERCOLLECTION_H
#define ROOMVARIABLECONTAINERCOLLECTION_H

#include <QObject>
#include <QMap>
#include "roomvariablecontainer.h"
#include "genericroomvariablecontainer.h"
#include "mapsettingsvariablecontainer.h"

namespace orpg
{
  class Message;
  class RoomVariableMessage;
}

class RoomVariableContainerCollection : public QObject
{
  Q_OBJECT
  QMap<QString,RoomVariableContainer*> containerMap;
  MapSettingsVariableContainer* mapSettingsContainer;

  void addContainer(RoomVariableContainer* container);
public:
  explicit RoomVariableContainerCollection(QObject *parent = nullptr);

  MapSettingsVariableContainer *getMapSettingsContainer() const;
  void setMapSettingsContainer(MapSettingsVariableContainer *value);
  orpg::RoomVariableMessage setMessage(const QString& variableName);
  QList<orpg::Message> getAllVariableMessages();

signals:
  void variableChanged(const QString& variableName);
private slots:
  void variableChangedInternal();
public slots:
  bool handleMessage(const orpg::RoomVariableMessage &msg);
  void announceVariables();
  void clear();
};

#endif // ROOMVARIABLECONTAINERCOLLECTION_H
