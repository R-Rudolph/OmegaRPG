#include "kickmessage.h"

namespace orpg
{
  const QString KickMessage::messageType = "kick";
  const QString KickMessage::idField = "kick";

  KickMessage::KickMessage(const QJsonObject& object) : AbstractMessage()
  {
    this->_id = object[idField].toString();
  }

  KickMessage::KickMessage(const QString& id) : AbstractMessage()
  {
    this->_id = id;
  }

  KickMessage::KickMessage(quint32 id) : KickMessage(QString::number(id))
  {

  }

  QJsonObject KickMessage::toJson() const
  {
    QJsonObject object;
    addToJson(object, actionField, messageType);
    addToJson(object,idField,_id);
    return object;
  }

  bool KickMessage::isValid() const
  {
    return !_id.isEmpty();
  }

  BaseMessage*KickMessage::clone() const
  {
    return new KickMessage(*this);
  }

  BaseMessage::MessageType KickMessage::type() const
  {
    return TypeKick;
  }

  QString KickMessage::id() const
  {
    return _id;
  }

  void KickMessage::setId(const QString& id)
  {
    _id = id;
  }

}
