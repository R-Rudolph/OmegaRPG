#ifndef GENERICRESOURCECONTAINER_H
#define GENERICRESOURCECONTAINER_H

#include "genericresource.h"
#include "resourcecontainer.h"

class GenericResourceContainer : public ResourceContainer
{
  QString resourceName;
protected:
  virtual Resource* newResource(const QJsonObject& data);
public:
  virtual const QString& name() const;
  explicit GenericResourceContainer(QObject *parent = nullptr);
  explicit GenericResourceContainer(QString resourceName, QObject *parent = nullptr);
  QString getResourceName() const;
  void setResourceName(const QString &value);
};

#endif // GENERICRESOURCECONTAINER_H
