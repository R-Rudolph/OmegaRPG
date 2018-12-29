#ifndef RESOURCECONTAINERCOLLECTION_H
#define RESOURCECONTAINERCOLLECTION_H

#include <QObject>
#include <QMap>
#include <QString>

class ResourceContainer;
class InitiativeResourceContainer;
class InitiativeResource;
class MapLineResourceContainer;
class MapFogResourceContainer;
class MapMiniatureResourceContainer;
class MapTextResourceContainer;
namespace orpg
{
  class Message;
  class RoomResourceMessage;
}

class ResourceContainerCollection : public QObject
{
  Q_OBJECT
  QMap<QString,ResourceContainer*> containerMap;
  InitiativeResourceContainer* initiativeContainer;
  MapLineResourceContainer* mapLineContainer;
  MapFogResourceContainer* mapFogContainer;
  MapMiniatureResourceContainer* mapMiniatureContainer;
  MapTextResourceContainer* mapTextContainer;
  void newContainer(ResourceContainer* container);
public:
  explicit ResourceContainerCollection(QObject *parent = nullptr);
  QList<orpg::Message> getAllResourceMessages();
  void clear();
  InitiativeResourceContainer *getInitiativeContainer() const;
  MapLineResourceContainer *getMapLineContainer() const;
  MapFogResourceContainer *getMapFogContainer() const;
  MapMiniatureResourceContainer *getMapMiniatureContainer() const;
  MapTextResourceContainer *getMapTextContainer() const;
signals:
  //generic
  void resourceChanged(ResourceContainer* container, QList<int> ids);
  void resourceRemoved(ResourceContainer* container, QList<int> ids);
  //initiative
  void initiativeChanged(const QList<InitiativeResource>& initiative);
public slots:
  bool handleMessage(const orpg::RoomResourceMessage &msg);
private slots:
  void resourceChangedInternal(QList<int> ids);
  void resourceRemovedInternal(QList<int> ids);
  void initiativeChangedInternal(QList<int> ids);
};

#endif // RESOURCECONTAINERCOLLECTION_H
