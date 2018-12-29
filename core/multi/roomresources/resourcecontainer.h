#ifndef RESOURCECONTAINER_H
#define RESOURCECONTAINER_H

#include <QObject>
#include <QJsonObject>
#include <QMap>
#include "resource.h"
#include "core/multi/util/player.h"
#include "core/multi/message/roomresourcemessage.h"

class GenericResource;

class ResourceContainer : public QObject
{
  Q_OBJECT
public:
  static const QString idField;
  static const QString actionField;
  static const QString nameField;
  static const QString dataField;
  static const QString setString;
  static const QString removeString;
protected:
  int idOffset;
  int maxId;
  bool reuseId;
  QMap<int, Resource*> resourceMap;
  virtual Resource* newResource(const QJsonObject& data)=0;
  virtual PlayerRole requiredEditStatus();
  virtual void beforeSetting(const Resource& resource);
  virtual void afterSetting(const Resource& resource);
  virtual void beforeRemoving(int resId);
  virtual void afterRemoving(int resId);
  virtual QList<int> findNextIds(int n);
  virtual bool validForContainer(const Resource& resource);
public:
  virtual const QString& name() const =0;
  explicit ResourceContainer(QObject *parent = nullptr);
  bool handleMessage(const orpg::RoomResourceMessage &msg);
  bool setResources(const QVector<GenericResource>& resources);
  bool removeResources(const QList<int>& resIds);
  virtual void clear();
  orpg::RoomResourceMessage removeMessage(int id) const;
  orpg::RoomResourceMessage removeMessage(const QList<int>& ids) const;
  orpg::RoomResourceMessage setMessage(int id) const;
  orpg::RoomResourceMessage setMessage(const QList<int>& ids) const;
  orpg::RoomResourceMessage setMessage(const Resource& resource) const;
  orpg::RoomResourceMessage setMessage(const QList<GenericResource>& resources) const;
  orpg::RoomResourceMessage newMessage(const Resource& resource) const;
  orpg::RoomResourceMessage newMessage(const QList<GenericResource>& resources) const;
  Resource* getResourceById(int id);
  orpg::RoomResourceMessage allMessages();
  orpg::RoomResourceMessage allRemoveMessages();
signals:
  void resourceChanged(QList<int> ids);
  void resourceRemoved(QList<int>  ids);
public slots:
};

#endif // RESOURCECONTAINER_H
