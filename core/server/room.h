#ifndef ROOM_H
#define ROOM_H

#include <QObject>
#include <QString>
#include "core/multi/message/message.h"
#include "core/multi/roomresources/resourcecontainercollection.h"
#include "core/multi/roomvariables/roomvariablecontainercollection.h"
#include "core/multi/util/roominfo.h"
#include<QDateTime>

namespace orpg
{

  class Room : public QObject
  {
    Q_OBJECT
    void sendMessageInternal(Message msg);
    explicit Room(QObject *parent);
  public:
    explicit Room(int id=-1, QString name="New Room", QString password=QString(), QString dmpassword=QString(), QObject* parent=nullptr, const QString& propertiesFilepath="");
    virtual ~Room();

    ResourceContainerCollection* resources;
    RoomVariableContainerCollection* variables;
    //internal properties
    int _id;
    QDateTime _time;
    QString _propertiesFilepath;
    //user controlled data
    QString _name;
    QString _password;
    QString _dmpassword;
    //statistical data
    int _numPlayers;

    RoomInfo info() const;
  private slots:
    void resourceChanged(ResourceContainer* container, QList<int> ids);
    void resourceRemoved(ResourceContainer* container, QList<int> ids);
    void variableChanged(const QString& variableName);
  signals:
    void sendMessage(Message msg);
  };

}

#endif // ROOM_H
