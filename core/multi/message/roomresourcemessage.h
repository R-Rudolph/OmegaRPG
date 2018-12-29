#ifndef ROOMRESOURCEMESSAGE_H
#define ROOMRESOURCEMESSAGE_H

#include "abstractmessage.h"
#include "core/multi/roomresources/resource.h"
#include "core/multi/roomresources/genericresource.h"
#include<QMap>

namespace orpg
{
  /*
   * Room Resource Message
   * Sent by clients and servers to add/edit/remove room resources
   */
  class RoomResourceMessage : public AbstractMessage
  {
  public:
    enum ActionType
    {
      ActionUnknown,
      ActionSet,
      ActionRemove
    };

    static const QString messageType;
    static const QString resourceActionField;
    static const QString resourceIdField;
    static const QString nameField;
    static const QString dataField;

    static const QString setString;
    static const QString removeString;
    static const QMap<RoomResourceMessage::ActionType,QString> actionStringMap;
    static const QMap<QString,RoomResourceMessage::ActionType> stringActionMap;
  protected:
    QList<int> _ids;
    ActionType _action;
    QString _name;
    QList<QJsonObject> _data;
    bool _isValid;

  public:
    RoomResourceMessage();
    RoomResourceMessage(const QJsonObject& object);
    static RoomResourceMessage newResourceMessage(const Resource& resource);
    static RoomResourceMessage newResourceMessage(const QList<GenericResource>& resources);
    static RoomResourceMessage setResourceMessage(const Resource& resource);
    static RoomResourceMessage setResourceMessage(const QList<GenericResource>& resources);
    static RoomResourceMessage removeResourceMessage(const Resource& resource);
    static RoomResourceMessage removeResourceMessage(const QList<GenericResource>& resources);
    static RoomResourceMessage removeResourceMessage(const QString& resourceName, const QList<int>& ids);
    static RoomResourceMessage removeResourceMessage(const QString& resourceName, int id);

    virtual QJsonObject toJson() const;
    virtual bool isValid() const;
    virtual BaseMessage* clone() const;
    virtual MessageType type() const;

    ActionType action() const;
    QString name() const;
    QList<int> ids() const;
    QList<GenericResource> resources() const;
    bool isNewMessage() const;
    bool isSetMessage() const;
    bool isRemoveMessage() const;
  };
}


#endif // ROOMRESOURCEMESSAGE_H
