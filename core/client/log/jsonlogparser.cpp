#include "jsonlogparser.h"

#include<QFile>
#include<QJsonDocument>
#include<QJsonArray>

#include "elements/logelement.h"

#include "elements/logchatelement.h"
#include "elements/loginfoelement.h"
#include "elements/logplayerelement.h"
#include "elements/logplayerremoveelement.h"
#include "elements/logstatuselement.h"

#include<QMetaType>
#include<QSharedPointer>

namespace orpg
{
  bool JsonLogParser::registered = false;

  QString JsonLogParser::getFilename() const
  {
    return filename;
  }

  void JsonLogParser::registerTypes()
  {
    if(!registered)
    {
      qRegisterMetaType<orpg::JsonLogParser>("orpg::JsonLogParser");
      qRegisterMetaType< QSharedPointer<orpg::JsonLogParser> >("QSharedPointer<orpg::JsonLogParser>");
    }
  }

  JsonLogParser::JsonLogParser()
  {
    infoColor = "#ff8000";
    error = JsonLogParser::Unknown;
    numberChatMessages = 0;
    idNameMap.insert("dm","DM");
    idNameMap.insert("all","Main Channel");
    idNameMap.insert("","Main Channel");
  }

  void JsonLogParser::parse(const QByteArray& data)
  {
    QJsonArray array = QJsonDocument::fromJson(data,&jsonParseError).array();
    if(jsonParseError.error == QJsonParseError::NoError)
    {
      elements.reserve(array.size());
      for(int i=0;i<array.size();i++)
      {
        LogElement element(array[i].toObject());
        if(element.isValid())
        {
          elements.append(element);
          switch(element.getElementType())
          {
            case LogElement::Info:
            {
              const LogInfoElement* subelement = element.infoElement();
              info = *subelement;
              break;
            }
            case LogElement::Chat:
            {
              numberChatMessages++;
              break;
            }
            case LogElement::Player:
            {
              const LogPlayerElement* subelement = element.playerElement();
              idNameMap.insert(subelement->getId(),subelement->getName());
              break;
            }
            default:
              break;
          }
        }
      }
      if(info.isValid())
      {
        error = JsonLogParser::NoError;
      }
      else
      {
        error = JsonLogParser::MalformedLogfileError;
      }
    }
    else
    {
      error = JsonLogParser::FileFormatError;
    }
  }

  JsonLogParser::~JsonLogParser()
  {

  }

  JsonLogParser::JsonLogParser(const QString& filename) : JsonLogParser()
  {
    this->filename = filename;
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly))
    {
      parse(file.readAll());
    }
    else
    {
      error = JsonLogParser::FileOpenError;
    }
  }

  JsonLogParser::JsonLogParser(const QByteArray& data) : JsonLogParser()
  {
    parse(data);
  }
}
