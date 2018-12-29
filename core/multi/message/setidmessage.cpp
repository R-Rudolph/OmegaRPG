#include "setidmessage.h"

namespace orpg
{
  const QString SetIdMessage::messageType = "setID";
  const QString SetIdMessage::idField = "id";

  QString SetIdMessage::id() const
  {
    return _id;
  }

  void SetIdMessage::setId(const QString& id)
  {
    _id = id;
  }

  SetIdMessage::SetIdMessage(const QJsonObject& object) : AbstractMessage()
  {
    _id = object[idField].toString();
  }

  SetIdMessage::SetIdMessage(const QString& id) : AbstractMessage()
  {
    this->_id = id;
  }

  SetIdMessage::SetIdMessage(quint32 id) : SetIdMessage(QString::number(id))
  {

  }

  QJsonObject SetIdMessage::toJson() const
  {
    QJsonObject object;
    addToJson(object, actionField, messageType);
    addToJson(object,idField,_id);
    return object;
  }

  bool SetIdMessage::isValid() const
  {
    return !_id.isEmpty();
  }

  BaseMessage* SetIdMessage::clone() const
  {
    return new SetIdMessage(*this);
  }

  BaseMessage::MessageType SetIdMessage::type() const
  {
    return TypeSetId;
  }
}

