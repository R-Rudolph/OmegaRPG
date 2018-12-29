#ifndef ROOMVARIABLEMESSAGE_H
#define ROOMVARIABLEMESSAGE_H

#include "abstractmessage.h"
#include "core/multi/roomvariables/roomvariable.h"

namespace orpg
{
  /*
   * Room Resource Message
   * Sent by clients and servers to set room variables
   */
  class RoomVariableMessage : public AbstractMessage
  {
  public:
    static const QString messageType;
    static const QString nameField;
    static const QString dataField;
  protected:
    QString _name;
    QJsonValue _data;
  public:
    RoomVariableMessage();
    RoomVariableMessage(const RoomVariable& variable);
    RoomVariableMessage(const QJsonObject& object);

    virtual QJsonObject toJson() const;
    virtual bool isValid() const;
    virtual BaseMessage* clone() const;
    virtual MessageType type() const;

    QString name() const;
    void setName(const QString& name);
    QJsonValue data() const;
    void setData(const QString& data);
  };
}


#endif // ROOMVARIABLEMESSAGE_H
