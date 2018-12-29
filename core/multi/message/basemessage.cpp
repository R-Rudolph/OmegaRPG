#include "basemessage.h"
#include<QJsonDocument>
#include<QJsonArray>

namespace orpg
{
  const QString BaseMessage::actionField = "action";

  BaseMessage::BaseMessage()
  {

  }

  void BaseMessage::addToJson(QJsonObject& object, const QString& field, const QJsonValue& value) const
  {
    if(value.isString() && value.toString().isEmpty())
      return;
    else if(value.isArray() & value.toArray().isEmpty())
      return;
    else if(value.isNull() || value.isUndefined())
      return;
    object.insert(field,value);
  }

  BaseMessage::~BaseMessage()
  {

  }

  QByteArray BaseMessage::toByteArray() const
  {
    return qCompress(QJsonDocument(toJson()).toJson(QJsonDocument::Compact),3);
  }
}
