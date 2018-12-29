#ifndef MAPLINERESOURCECONTAINER_H
#define MAPLINERESOURCECONTAINER_H

#include "../resourcecontainer.h"

class MapLineResource;

class MapLineResourceContainer : public ResourceContainer
{
  Q_OBJECT
protected:
  PlayerRole requiredEditStatus();
  virtual Resource* newResource(const QJsonObject& data);
public:
  const QString &name() const;
  explicit MapLineResourceContainer(QObject *parent = nullptr);
  MapLineResource* getLineResource(int id);
};

#endif // MAPLINERESOURCECONTAINER_H
