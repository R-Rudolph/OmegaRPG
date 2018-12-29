#ifndef INITIATIVEQUEUERESOURCE_H
#define INITIATIVEQUEUERESOURCE_H

#include <QString>
#include <QJsonObject>
#include "resource.h"

class InitiativeResource : public Resource
{
public:
  static const QString initiativeName;
private:
  QString _name;
  QString status;
  QString iconUrl;
  QString color;
  bool isActive;
  int initiative;
  int position;
public:
  InitiativeResource(const QString& name=QString(), const QString& status=QString(), const QString& iconUrl=QString(), bool isActive=false, int initiative=0, int position=0, const QString& color=QString("#000000"));
  InitiativeResource(const QJsonObject& data);
  QJsonObject data() const;
  bool operator <(const InitiativeResource& other) const;
  static bool initiativeGreater(const InitiativeResource& a, const InitiativeResource& b);
  QString getName() const;
  void setName(const QString &value);
  QString getStatus() const;
  void setStatus(const QString &value);
  QString getIconUrl() const;
  void setIconUrl(const QString &value);
  bool getIsActive() const;
  void setIsActive(bool value);
  int getInitiative() const;
  void setInitiative(int value);
  int getPosition() const;
  void setPosition(int value);
  QString getColor() const;
  void setColor(const QString &value);
  virtual const QString& name() const;
};

#endif // INITIATIVEQUEUERESOURCE_H
