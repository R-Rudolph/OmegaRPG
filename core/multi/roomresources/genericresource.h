#ifndef GENERICRESOURCE_H
#define GENERICRESOURCE_H

#include <QJsonObject>
#include "resource.h"

class GenericResource : public Resource
{
  QJsonObject _jsonData;
  QString _name;
public:
  GenericResource();
  GenericResource(const QJsonObject& data);
  GenericResource(const Resource& resource);
  virtual QJsonObject data() const;
  virtual const QString& name() const;
};

#endif // GENERICRESOURCE_H
