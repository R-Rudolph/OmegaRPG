#ifndef MAPTEXTRESOURCECONTAINER_H
#define MAPTEXTRESOURCECONTAINER_H

#include <QObject>
#include "../resourcecontainer.h"
class MapTextResource;

class MapTextResourceContainer : public ResourceContainer
{
  Q_OBJECT
protected:
  PlayerRole requiredEditStatus();
  virtual Resource* newResource(const QJsonObject& data);
public:
  const QString &name() const;
  explicit MapTextResourceContainer(QObject *parent = nullptr);
  MapTextResource* getTextResource(int id);
};

#endif // MAPTEXTRESOURCECONTAINER_H
