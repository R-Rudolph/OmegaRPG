#ifndef SERVERENTRY_H
#define SERVERENTRY_H

#include <QString>
#include <QJsonObject>

class ServerEntry
{
public:
  QString address;
  QString name;
  int players;
  ServerEntry(QString address="", QString name="");
  ServerEntry(const QJsonObject& obj);
  QJsonObject toJson() const;
};

#endif // SERVERENTRY_H
