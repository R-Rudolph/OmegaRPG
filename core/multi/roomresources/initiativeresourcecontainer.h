#ifndef INITIATIVERESOURCECONTAINER_H
#define INITIATIVERESOURCECONTAINER_H

#include<QList>
#include "resourcecontainer.h"
#include "initiativeresource.h"

class InitiativeResourceContainer : public ResourceContainer
{
  Q_OBJECT
  int findIdByPosition(int position, int excludeId);
protected:
  virtual Resource* newResource(const QJsonObject& data);
  virtual void afterSetting(const Resource& resource);
public:
  virtual const QString& name() const;
  InitiativeResourceContainer(QObject* parent = nullptr);
  QList<InitiativeResource> getResources() const;
  QList<InitiativeResource> getResourcesByInitiative() const;
  orpg::RoomResourceMessage activateMessage(int id) const;
  orpg::RoomResourceMessage moveMessage(int id, int position);
  orpg::RoomResourceMessage sortMessage();
  orpg::RoomResourceMessage nextMessage();
};

#endif // INITIATIVERESOURCECONTAINER_H
