#ifndef SERVERLIST_H
#define SERVERLIST_H

#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include <QDomDocument>
#include "serverentry.h"

class ServerList
{
  QVector<ServerEntry> entries;
public:
  ServerList();
  ServerList(QJsonArray serverArray);
  void addEntry(const ServerEntry& entry);
  void clear();
  int size() const;
  const ServerEntry& at(int index) const;
  const ServerEntry& operator[](int index) const;
  QJsonArray toJson() const;
  bool parseXml(const QByteArray& xml);
  void insert(const ServerList& list);
  bool contains(const ServerEntry& entry) const;
};

#endif // SERVERLIST_H
