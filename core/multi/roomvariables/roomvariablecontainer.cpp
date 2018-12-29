#include "roomvariablecontainer.h"
#include<QJsonObject>

const RoomVariable *RoomVariableContainer::getAbstractVariable() const
{
  return variable;
}

void RoomVariableContainer::forceAnnounceChange()
{
  emit variableChanged();
}

PlayerRole RoomVariableContainer::requiredEditStatus()
{
  return ROLE_GM;
}

void RoomVariableContainer::beforeSetting(const RoomVariable&)
{

}

void RoomVariableContainer::afterSetting(const RoomVariable&)
{

}

void RoomVariableContainer::initializeVariable()
{
  variable = newResource();
}

RoomVariableContainer::RoomVariableContainer(QObject *parent) : QObject(parent)
{
  variable = nullptr;
}

bool RoomVariableContainer::handleMessage(const orpg::RoomVariableMessage& msg)
{
  if(msg.senderRole()<requiredEditStatus() && msg.hasSenderId())
    return false;
  return handleMessage(msg.name(),msg.data());
}

bool RoomVariableContainer::handleMessage(const QString &resourceName, const QJsonValue &data)
{
  if(resourceName!=name())
    return false;
  RoomVariable* variable = this->newResource(resourceName,data);
  if(variable->isValid())
  {
    beforeSetting(*variable);
    delete this->variable;
    this->variable = variable;
    afterSetting(*variable);
    emit variableChanged();
  }
  else
    return false;
  return true;
}

void RoomVariableContainer::reset()
{
  delete variable;
  initializeVariable();
}

orpg::RoomVariableMessage RoomVariableContainer::setMessage() const
{
  return orpg::RoomVariableMessage(*variable);
}
