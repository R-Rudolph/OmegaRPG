#include "createroommessage.h"

namespace orpg
{
  const QString CreateRoomMessage::messageType = "createRoom";
  const QString CreateRoomMessage::nameField = "name";
  const QString CreateRoomMessage::passwordField = "pass";
  const QString CreateRoomMessage::dmPasswordField = "dmpass";

  QString CreateRoomMessage::name() const
  {
    return _name;
  }

  void CreateRoomMessage::setName(const QString& name)
  {
    _name = name;
  }

  QString CreateRoomMessage::password() const
  {
    return _password;
  }

  void CreateRoomMessage::setPassword(const QString& password)
  {
    _password = password;
  }

  QString CreateRoomMessage::dmPassword() const
  {
    return _dmPassword;
  }

  void CreateRoomMessage::setDmPassword(const QString& dmPassword)
  {
    _dmPassword = dmPassword;
  }

  CreateRoomMessage::CreateRoomMessage(const QString& name, const QString& password, const QString& dmPassword) : AbstractMessage()
  {
    _name = name;
    _password = password;
    _dmPassword = dmPassword;
  }

  CreateRoomMessage::CreateRoomMessage(const QJsonObject& object) : AbstractMessage()
  {
    _name = object[nameField].toString();
    _password = object[passwordField].toString();
    _dmPassword = object[dmPasswordField].toString();
  }

  QJsonObject CreateRoomMessage::toJson() const
  {
    QJsonObject object;
    addToJson(object,actionField,messageType);
    addToJson(object,nameField,_name);
    addToJson(object,passwordField,_password);
    addToJson(object,dmPasswordField,_dmPassword);
    return object;
  }

  bool CreateRoomMessage::isValid() const
  {
    return !_name.isEmpty();
  }

  BaseMessage* CreateRoomMessage::clone() const
  {
    return new CreateRoomMessage(*this);
  }

  BaseMessage::MessageType CreateRoomMessage::type() const
  {
    return TypeCreateRoom;
  }

}
