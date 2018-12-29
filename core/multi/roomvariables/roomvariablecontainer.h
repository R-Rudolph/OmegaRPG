#ifndef ROOMVARIABLECONTAINER_H
#define ROOMVARIABLECONTAINER_H

#include <QObject>
#include "roomvariable.h"
#include "core/multi/message/roomvariablemessage.h"

class RoomVariableContainer : public QObject
{
  Q_OBJECT
protected:
  RoomVariable* variable;
  virtual RoomVariable* newResource()=0;
  virtual RoomVariable* newResource(const QString& resourceName, const QJsonValue& data)=0;
  virtual PlayerRole requiredEditStatus();
  virtual void beforeSetting(const RoomVariable& variable);
  virtual void afterSetting(const RoomVariable& variable);
  void initializeVariable();
public:
  virtual const QString& name() const =0;
  explicit RoomVariableContainer(QObject *parent = nullptr);
  bool handleMessage(const orpg::RoomVariableMessage &msg);
  bool handleMessage(const QString& resourceName, const QJsonValue &data);
  void reset();
  orpg::RoomVariableMessage setMessage() const;
  const RoomVariable *getAbstractVariable() const;
  void forceAnnounceChange();
signals:
  void variableChanged();

public slots:
};

#endif // ROOMVARIABLECONTAINER_H
