#ifndef MAPMINIATURERESOURCECONTAINER_H
#define MAPMINIATURERESOURCECONTAINER_H

#include <QObject>
#include "../resourcecontainer.h"
class MapMiniatureResource;

class MapMiniatureResourceContainer : public ResourceContainer
{
  Q_OBJECT
protected:
  PlayerRole requiredEditStatus();
  virtual Resource* newResource(const QJsonObject& data);
public:
  const QString &name() const;
  explicit MapMiniatureResourceContainer(QObject *parent = nullptr);
  MapMiniatureResource* getMiniatureResource(int id);
};

#endif // MAPMINIATURERESOURCECONTAINER_H
