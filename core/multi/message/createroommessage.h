#ifndef CREATEROOMMESSAGE_H
#define CREATEROOMMESSAGE_H

#include "abstractmessage.h"

namespace orpg
{
  /*
   * Create Room Message
   * Sent by clients to create a new room
   */
  class CreateRoomMessage : public AbstractMessage
  {
  public:
    static const QString messageType;
    static const QString nameField;
    static const QString passwordField;
    static const QString dmPasswordField;
  protected:
    QString _name;
    QString _password;
    QString _dmPassword;
  public:
    CreateRoomMessage(const QString& name, const QString& password, const QString& dmPassword);
    CreateRoomMessage(const QJsonObject& object);

    virtual QJsonObject toJson() const;
    virtual bool isValid() const;
    virtual BaseMessage* clone() const;
    virtual MessageType type() const;

    QString name() const;
    void setName(const QString& name);
    QString password() const;
    void setPassword(const QString& password);
    QString dmPassword() const;
    void setDmPassword(const QString& dmPassword);
  };
}


#endif // CREATEROOMMESSAGE_H
