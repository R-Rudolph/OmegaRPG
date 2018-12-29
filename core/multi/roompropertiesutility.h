#ifndef ROOMPROPERTIESUTILITY_H
#define ROOMPROPERTIESUTILITY_H

#include "roomvariables/roomvariablecontainercollection.h"
#include "roomresources/resourcecontainercollection.h"

namespace orpg
{
  class Message;
}

class RoomPropertiesUtility
{
  static const QString variableString;
  static const QString resourceString;
public:
  static bool saveRoomProperties(RoomVariableContainerCollection* variableCollection, ResourceContainerCollection* resourceCollection, const QString& filepath);
  static bool loadRoomProperties(RoomVariableContainerCollection *variableCollection, ResourceContainerCollection *resourceCollection, const QString &filepath);
  static QList<orpg::Message> loadRoomProperties(const QString& filepath);
};

#endif // ROOMPROPERTIESUTILITY_H
