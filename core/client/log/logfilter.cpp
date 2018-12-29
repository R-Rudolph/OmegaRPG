#include "logfilter.h"

#include "core/client/log/logparser.h"
#include "core/client/log/elements/logelement.h"
#include "core/client/log/elements/logchatelement.h"

#include<QSharedPointer>
#include<QMetaType>

namespace orpg
{
  bool LogFilter::registered = false;

  QString LogFilter::descriptiveString(LogFilter::FilterField field)
  {
    switch(field)
    {
      case FIELD_ALIAS:
        return "Alias";
      case FIELD_ROOM:
        return "Room";
      case FIELD_TEXT:
        return "Text";
      default:
        return "Invalid";
    }
  }

  QString LogFilter::descriptiveString(LogFilter::MatchType match)
  {
    switch(match)
    {
      case MATCH_EXACT:
        return "Exact Match";
      case MATCH_PARTIAL:
        return "Partial Match";
      case MATCH_REGEX:
        return "Regular Expression";
      case MATCH_WORDS:
        return "Contains Words";
      default:
        return "Invalid";
    }
  }

  QString LogFilter::shortString(LogFilter::MatchType match)
  {
    switch(match)
    {
      case MATCH_EXACT:
        return "Exact";
      case MATCH_PARTIAL:
        return "Partial";
      case MATCH_REGEX:
        return "RegEx";
      case MATCH_WORDS:
        return "Words";
      default:
        return "Invalid";
    }
  }

  void LogFilter::setFilterString(const QString &value)
  {
    filterString = value;
    switch(match)
    {
      case MATCH_REGEX:
        regex = QRegularExpression(filterString,QRegularExpression::CaseInsensitiveOption);
        regex.optimize();
        break;
      case MATCH_WORDS:
        words = filterString.split(' ',QString::SkipEmptyParts,Qt::CaseInsensitive);
        break;
      default:
        break;
    }
  }

  bool LogFilter::matches(const QString &text) const
  {
    switch(match)
    {
      case MATCH_EXACT:
        return QString::compare(text,filterString,Qt::CaseInsensitive) == 0;
      case MATCH_PARTIAL:
        return text.contains(filterString,Qt::CaseInsensitive);
      case MATCH_REGEX:
        return regex.match(text).hasMatch();
      case MATCH_WORDS:
        foreach(const QString& word, words)
        {
          if(!text.contains(word))
            return false;
        }
        return true;
      default:
        return true;
    }
  }

  LogFilter::LogFilter()
  {
    field = FIELD_INVALID;
    match = MATCH_INVALID;
    inverse = false;
  }

  LogFilter::LogFilter(const QList<QString>& logUIDs) : LogFilter()
  {
    this->field = FIELD_MANUAL_SELECTION;
    selectionList = logUIDs;
  }

  LogFilter::LogFilter(LogFilter::FilterField field, LogFilter::MatchType match, const QString &filterString, bool inverse)
    : LogFilter()
  {
    this->field = field;
    this->match = match;
    this->inverse = inverse;
    setFilterString(filterString);
  }

  QString LogFilter::description() const
  {
    if(field==FIELD_MANUAL_SELECTION)
      return "Manual Filter";
    QString filterStringShort = filterString;
    if(filterStringShort.length()>20)
      filterStringShort = filterStringShort.left(17)+"...";
    return (inverse ? "¬" : "")+descriptiveString(field) + ":" + shortString(match) + ":" + filterStringShort;
  }

  bool LogFilter::matches(LogParser& logParser) const
  {
    switch(field)
    {
      case FIELD_ROOM:
        return matches(logParser.getRoomName())^inverse;
      case FIELD_ALIAS:
        foreach(const LogElement& element, logParser.getElements())
        {
          if(element.isChatElement() && matches(element.chatElement()->getName()))
            return !inverse;
        }
        return inverse;
      case FIELD_TEXT:
        foreach(const LogElement& element, logParser.getElements())
        {
          if(element.isChatElement() && matches(element.chatElement()->getText()))
            return !inverse;
        }
        return inverse;
      case FIELD_MANUAL_SELECTION:
        return !selectionList.contains(logParser.getUID());
      default:
        return true;
    }
  }

  bool LogFilter::operator ==(const LogFilter& other)
  {
    return (field==other.field) &&
           (match==other.match) &&
           (inverse == other.inverse) &&
        (filterString == other.filterString);
  }

  void LogFilter::registerTypes()
  {
    if(!registered)
    {
      qRegisterMetaType<orpg::LogFilter>("orpg::LogFilter");
      qRegisterMetaType< QSharedPointer<orpg::LogFilter> >("QSharedPointer<orpg::LogFilter>");
    }
  }
}
