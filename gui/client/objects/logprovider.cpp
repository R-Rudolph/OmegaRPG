#include "logprovider.h"
#include<QMutableListIterator>
#include<QDir>
#include "core/client/settings.h"


void LogProvider::reevaluateAllFilters()
{
  filteredLogs.clear();
  QMutableListIterator< QSharedPointer<orpg::JsonLogParser> > it(allLogs);
  foreach(const QSharedPointer<orpg::JsonLogParser>& log, allLogs)
  {
    if(matchesFilters(log))
    {
      filteredLogs.append(log);
      emit foundLog(log);
    }
  }
}

bool LogProvider::matchesFilters(const QSharedPointer<orpg::JsonLogParser>& log)
{
  foreach(const orpg::LogFilter& filter, filters)
  {
    if(!filter.matches(*log))
      return false;
  }
  return true;
}

bool LogProvider::logOrder(const QSharedPointer<orpg::JsonLogParser>& log1, const QSharedPointer<orpg::JsonLogParser>& log2)
{
  return log1->getTime() < log2->getTime();
}

LogProvider::LogProvider(QObject *parent) : QObject(parent)
{

}

void LogProvider::refresh()
{
  emit started();
  allLogs.clear();
  QDir dir(orpg::Settings::get()->getLogLocation(),"*.json",QDir::Name|QDir::IgnoreCase,QDir::Files);
  foreach(QString file, dir.entryList())
  {
    QSharedPointer<orpg::JsonLogParser> log(new orpg::JsonLogParser(dir.filePath(file)));
    if(log->isValid())
    {
      allLogs.append(log);
    }
  }
  std::sort(allLogs.begin(),allLogs.end(),LogProvider::logOrder);
  reevaluateAllFilters();
  emit finished();
}

void LogProvider::addFilter(orpg::LogFilter filter)
{
  emit started();
  filters.append(filter);
  QMutableListIterator< QSharedPointer<orpg::JsonLogParser> > it(filteredLogs);
  while (it.hasNext())
  {
    QSharedPointer<orpg::JsonLogParser> log = it.next();
    if(filter.matches(*log))
    {
      emit foundLog(log);
    }
    else
    {
      it.remove();
    }
  }
  emit finished();
}

void LogProvider::removeFilter(orpg::LogFilter filter)
{
  emit started();
  filters.removeOne(filter);
  filteredLogs.clear();
  reevaluateAllFilters();
  emit finished();
}

void LogProvider::removeAllFilters()
{
  emit started();
  filteredLogs = allLogs;
  foreach(const QSharedPointer<orpg::JsonLogParser>& log, filteredLogs)
  {
    emit foundLog(log);
  }
  emit finished();
}
