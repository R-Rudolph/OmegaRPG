#include "chatmessage.h"

namespace orpg
{
  const QString ChatMessage::messageType = "chat";
  const QString ChatMessage::toField = "to";
  const QString ChatMessage::nameField = "name";
  const QString ChatMessage::colorField = "color";
  const QString ChatMessage::textField = "text";

  const QString ChatMessage::allString = "all";
  const QString ChatMessage::dmString = "dm";

  const QVector<QString> ChatMessage::allowedXmlTags({"span"});
  const QVector<QString> ChatMessage::allowedXmlAttributes({"style"});

  QString ChatMessage::to() const
  {
    if(_to.isEmpty())
      return "all";
    else
      return _to;
  }

  QString ChatMessage::name() const
  {
    return _name;
  }

  QString ChatMessage::color() const
  {
    return _color;
  }

  QString ChatMessage::text() const
  {
    return _text;
  }

  bool ChatMessage::isToAll() const
  {
    return _to.isEmpty() || _to==allString;
  }

  bool ChatMessage::isToGm() const
  {
    return _to==dmString;
  }

  bool ChatMessage::isToId() const
  {
    bool ok;
    _to.toInt(&ok);
    return ok;
  }

  bool ChatMessage::isTo(const Player& player) const
  {
    return isToAll() ||
           (_senderId == player.id) ||
           (_to == player.id) ||
           (isToGm() && player.role==ROLE_GM);
  }

  void ChatMessage::stripHtml(QXmlStreamReader& reader, QXmlStreamWriter& writer, int depth)
  {
    while(!reader.atEnd())
    {
      reader.readNext();
      switch(reader.tokenType())
      {
        case QXmlStreamReader::StartDocument:
          break;
        case QXmlStreamReader::StartElement:
          if(allowedXmlTags.contains(reader.name().toString()))
          {
            writer.writeStartElement(reader.name().toString());
            QXmlStreamAttributes attributes = reader.attributes();
            foreach(const QString& attribute, allowedXmlAttributes)
            {
              if(attributes.hasAttribute(attribute))
              {
                writer.writeAttribute(attribute,attributes.value(attribute).toString());
              }
            }
            stripHtml(reader,writer,depth+1);
          }
          break;
        case QXmlStreamReader::Characters:
          writer.writeCharacters(reader.text().toString());
          break;
        case QXmlStreamReader::EndElement:
          if(allowedXmlTags.contains(reader.name().toString()))
          {
            writer.writeEndElement();
          }
          break;
        case QXmlStreamReader::EndDocument:
        case QXmlStreamReader::Comment:
        case QXmlStreamReader::DTD:
        case QXmlStreamReader::EntityReference:
        case QXmlStreamReader::ProcessingInstruction:
        case QXmlStreamReader::NoToken:
        case QXmlStreamReader::Invalid:
          return;
      }
    }
  }

  QString ChatMessage::stripHtml(const QString& text)
  {
    QString stripped;
    {
      QXmlStreamReader reader("<xml>"+text+"</xml>");
      QXmlStreamWriter writer(&stripped);
      writer.setAutoFormatting(false);
      stripHtml(reader,writer);
      writer.writeEndDocument();
      stripped.chop(1);
    }
    return stripped;
  }

  ChatMessage::ChatMessage(const QString& to, const QString& name, const QString& color, const QString& text, const QString& id) : AbstractMessage()
  {
    setSenderId(id);
    this->_to = to;
    this->_name = name;
    this->_color = color;
    this->_text = text;
  }

  ChatMessage::ChatMessage(const QJsonObject& object) : AbstractMessage(object)
  {
    this->_to = object[toField].toString();
    this->_name = stripHtml(object[nameField].toString());
    this->_color = object[colorField].toString().toHtmlEscaped();
    this->_text = stripHtml(object[textField].toString());
  }

  QJsonObject ChatMessage::toJson() const
  {
    QJsonObject object = AbstractMessage::toJson();
    addToJson(object, actionField, messageType);
    addToJson(object, toField, _to);
    addToJson(object, nameField, _name);
    addToJson(object, colorField, _color);
    addToJson(object, textField, _text);
    return object;
  }

  bool ChatMessage::isValid() const
  {
    return !_text.isEmpty() && !_name.isEmpty();
  }

  BaseMessage* ChatMessage::clone() const
  {
    return new ChatMessage(*this);
  }

  BaseMessage::MessageType ChatMessage::type() const
  {
    return TypeChatMessage;
  }
}
