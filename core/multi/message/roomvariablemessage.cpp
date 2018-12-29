#include "roomvariablemessage.h"

namespace orpg
{
  const QString RoomVariableMessage::messageType = "rvariable";
  const QString RoomVariableMessage::nameField = "variable";
  const QString RoomVariableMessage::dataField = "data";

  QString RoomVariableMessage::name() const
  {
    return _name;
  }

  void RoomVariableMessage::setName(const QString& name)
  {
    _name = name;
  }

  QJsonValue RoomVariableMessage::data() const
  {
    return _data;
  }

  void RoomVariableMessage::setData(const QString& data)
  {
    _data = data;
  }

  RoomVariableMessage::RoomVariableMessage()
  {

  }

  RoomVariableMessage::RoomVariableMessage(const RoomVariable& variable) : AbstractMessage()
  {
    _name = variable.name();
    _data = variable.data();
  }

  RoomVariableMessage::RoomVariableMessage(const QJsonObject& object) : AbstractMessage(object)
  {
    _name = object[nameField].toString();
    _data = object[dataField];
  }

  QJsonObject RoomVariableMessage::toJson() const
  {
    QJsonObject object = AbstractMessage::toJson();
    addToJson(object,actionField, messageType);
    addToJson(object,nameField,_name);
    addToJson(object,dataField,_data);
    return object;
  }

  bool RoomVariableMessage::isValid() const
  {
    return !_name.isEmpty() && !_data.isUndefined();
  }

  BaseMessage* RoomVariableMessage::clone() const
  {
    return new RoomVariableMessage(*this);
  }

  BaseMessage::MessageType RoomVariableMessage::type() const
  {
    return TypeRoomVariable;
  }

}
