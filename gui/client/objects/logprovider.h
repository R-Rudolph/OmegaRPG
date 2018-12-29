#ifndef LOGPROVIDER_H
#define LOGPROVIDER_H

#include <QObject>
#include <QTreeWidgetItem>
#include <QSharedPointer>
#include "core/client/log/jsonlogparser.h"
#include "core/client/log/logfilter.h"

class LogProvider : public QObject
{
  Q_OBJECT
  QList<orpg::LogFilter> filters;
  QList< QSharedPointer<orpg::JsonLogParser> > allLogs;
  QList< QSharedPointer<orpg::JsonLogParser> > filteredLogs;
  void reevaluateAllFilters();
  bool matchesFilters(const QSharedPointer<orpg::JsonLogParser>& log);
  static bool logOrder(const QSharedPointer<orpg::JsonLogParser> &log1, const QSharedPointer<orpg::JsonLogParser>& log2);
public:
  explicit LogProvider(QObject *parent = nullptr);
signals:
  void foundLog(QSharedPointer<orpg::JsonLogParser> log);
  void started();
  void finished();
public slots:
  void refresh();
  void addFilter(orpg::LogFilter filter);
  void removeFilter(orpg::LogFilter filter);
  void removeAllFilters();
};

#endif // LOGPROVIDER_H
