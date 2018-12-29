#include "unknownmessage.h"

namespace orpg
{

  UnknownMessage::UnknownMessage(const QJsonObject& object) : AbstractMessage()
  {
    _data = object;
  }

  QJsonObject UnknownMessage::toJson() const
  {
    return _data;
  }

  bool UnknownMessage::isValid() const
  {
    return false;
  }

  BaseMessage*UnknownMessage::clone() const
  {
    return new UnknownMessage(*this);
  }

  BaseMessage::MessageType UnknownMessage::type() const
  {
    return TypeUnknown;
  }

}
