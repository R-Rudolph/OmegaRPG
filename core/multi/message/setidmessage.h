#ifndef SETIDMESSAGE_H
#define SETIDMESSAGE_H

#include "abstractmessage.h"

namespace orpg
{
  /*
   * Set ID Message
   * Sent by servers on connecting to tell a client their ID
   */
  class SetIdMessage : public AbstractMessage
  {
  public:
    static const QString messageType;
    static const QString idField;
  protected:
    QString _id;
  public:
    SetIdMessage(const QJsonObject& object);
    SetIdMessage(const QString& id);
    SetIdMessage(quint32 id);

    virtual QJsonObject toJson() const;
    virtual bool isValid() const;
    virtual BaseMessage* clone() const;
    virtual MessageType type() const;

    QString id() const;
    void setId(const QString& id);
  };
}


#endif // SETIDMESSAGE_H
