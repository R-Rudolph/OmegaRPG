#ifndef UNKNOWNMESSAGE_H
#define UNKNOWNMESSAGE_H

#include "abstractmessage.h"

namespace orpg
{
  /*
   * Unknown Message
   * This message is not sent. Incoming messages of unknown type (e.g. sent by peers with incompatible protocol versions)
   * are treated es UnknownMessage types. This class simply wraps the QJsonObject it is constructed with.
   */
  class UnknownMessage : public AbstractMessage
  {
    QJsonObject _data;
  public:
    UnknownMessage(const QJsonObject& object);

    virtual QJsonObject toJson() const;
    virtual bool isValid() const;
    virtual BaseMessage* clone() const;
    virtual MessageType type() const;
  };
}


#endif // UNKNOWNMESSAGE_H
