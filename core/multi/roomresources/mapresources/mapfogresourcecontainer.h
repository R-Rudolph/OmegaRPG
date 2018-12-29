#ifndef MAPFOGRESOURCECONTAINER_H
#define MAPFOGRESOURCECONTAINER_H

#include <QObject>
#include "../resourcecontainer.h"
#include "mapfogresource.h"

class MapFogResource;

class MapFogResourceContainer : public ResourceContainer
{
  Q_OBJECT
  QMap<PointWrapper,int> positionMap;
protected:
  PlayerRole requiredEditStatus();
  virtual Resource* newResource(const QJsonObject& data);
  virtual void afterSetting(const Resource& resource);
  virtual void beforeRemoving(int resId);
  virtual bool validForContainer(const Resource& resource);
public:
  const QString &name() const;
  explicit MapFogResourceContainer(QObject *parent = nullptr);
  MapFogResource* getFogResource(int id);
};

#endif // MAPFOGRESOURCECONTAINER_H
