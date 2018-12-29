#ifndef LOGFILTER2_H
#define LOGFILTER2_H

#include <QString>
#include <QStringList>
#include <QRegularExpression>


namespace orpg
{
  class LogParser;

  class LogFilter
  {
    static bool registered;
  public:
    enum FilterField
    {
      FIELD_INVALID,
      FIELD_ROOM,
      FIELD_ALIAS,
      FIELD_TEXT,
      FIELD_MANUAL_SELECTION
    };
    enum MatchType
    {
      MATCH_INVALID,
      MATCH_PARTIAL,
      MATCH_EXACT,
      MATCH_WORDS,
      MATCH_REGEX
    };
    static QString descriptiveString(FilterField field);
    static QString descriptiveString(MatchType match);
    static QString shortString(MatchType match);
  private:
    QRegularExpression regex;
    QStringList words;
    FilterField field;
    MatchType match;
    QString filterString;
    QList<QString> selectionList;
    bool inverse;
    void setFilterString(const QString &value);
    bool matches(const QString& text) const;
  public:
    LogFilter();
    LogFilter(const QList<QString>& logUIDs);
    LogFilter(FilterField field, MatchType match, const QString& filterString, bool inverse);
    QString description() const;
    bool matches(LogParser& logParser) const;
    bool operator ==(const orpg::LogFilter& other);
    static void registerTypes();
  };
}


#endif // LOGFILTER2_H
