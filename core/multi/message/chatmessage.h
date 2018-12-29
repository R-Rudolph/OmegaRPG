#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include "abstractmessage.h"
#include "core/multi/util/player.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace orpg
{
  /*
   * Chat Message
   * Sent by clients and servers to pass chat messages
   */
  class ChatMessage : public AbstractMessage
  {
  public:
    static const QString messageType;
    static const QString toField;
    static const QString nameField;
    static const QString colorField;
    static const QString textField;

    static const QString allString;
    static const QString dmString;

    static const QVector<QString> allowedXmlTags;
    static const QVector<QString> allowedXmlAttributes;
  protected:
    QString _id;
    QString _to;
    QString _name;
    QString _color;
    QString _text;
    static void stripHtml(QXmlStreamReader& reader, QXmlStreamWriter& writer, int depth=0);
    static QString stripHtml(const QString& text);
  public:
    ChatMessage(const QString& to, const QString& name, const QString& color, const QString& text, const QString& id=QString());
    ChatMessage(const QJsonObject& object);

    virtual QJsonObject toJson() const;
    virtual bool isValid() const;
    virtual BaseMessage* clone() const;
    virtual MessageType type() const;

    QString to() const;
    QString name() const;
    QString color() const;
    QString text() const;
    bool isToAll() const;
    bool isToGm() const;
    bool isToId() const;
    bool isTo(const Player& player) const;
  };
}


#endif // CHATMESSAGE_H
