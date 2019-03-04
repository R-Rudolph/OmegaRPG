#include "ChannelName.h"
#include "logparser.h"

namespace orpg
{
  QString ChannelName::toNameWithId(const QString& id, const QMap<QString, QString>& mapping) const
  {
    return "("+id+") "+mapping[id];
  }

  ChannelName::ChannelName()
  {

  }

  ChannelName::ChannelName(const QString& from, const QString& to)
  {
    if(to.isEmpty())
    {
      if(from==LogParser::mixedString)
        type_ = ChannelMixed;
      else if(from==LogParser::gmString)
        type_ = ChannelGM;
      else
        type_ = ChannelAll;
    }
    else
    {
      if(to==LogParser::allString || from.isEmpty())
        type_ = ChannelAll;
      else if(to==LogParser::gmString)
        type_ = ChannelGM;
      else
      {
        type_ = ChannelPrivate;
        first_ = std::min(from,to);
        second_ = std::max(from,to);
      }
    }
  }

  QString ChannelName::rawName() const
  {
    switch(type_)
    {
      case ChannelAll:     return LogParser::allString;
      case ChannelGM:      return LogParser::gmString;
      case ChannelMixed:   return LogParser::mixedString;
      case ChannelPrivate: return first_+"/"+second_;
    }
    return QString();
  }

  QString ChannelName::name(const QMap<QString, QString>& mapping) const
  {
    switch(type_)
    {
      case ChannelAll:     return LogParser::mainChannelString;
      case ChannelGM:      return LogParser::gmChannelString;
      case ChannelMixed:   return LogParser::mixedChannelString;
      case ChannelPrivate: return toNameWithId(first_,mapping)+" - "+toNameWithId(second_,mapping);
    }
    return QString();
  }

  bool ChannelName::isAll() const
  {
    return type_==ChannelAll;
  }

  bool ChannelName::isGM() const
  {
    return type_==ChannelGM;
  }

  bool ChannelName::isMixed() const
  {
    return type_==ChannelMixed;
  }

  bool ChannelName::isPrivate() const
  {
    return type_==ChannelPrivate;
  }

  bool ChannelName::operator<(const ChannelName& other) const
  {
    if(type_==other.type_)
    {
      return rawName()<other.rawName();
    }
    else
    {
      return type_<other.type_;
    }
  }
}
