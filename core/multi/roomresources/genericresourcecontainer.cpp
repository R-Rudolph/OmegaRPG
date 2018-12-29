#include "genericresourcecontainer.h"

GenericResourceContainer::GenericResourceContainer(QObject *parent)
  : ResourceContainer(parent)
{

}

GenericResourceContainer::GenericResourceContainer(QString resourceName, QObject *parent)
  : GenericResourceContainer(parent)
{
  setResourceName(resourceName);
}

QString GenericResourceContainer::getResourceName() const
{
  return resourceName;
}

void GenericResourceContainer::setResourceName(const QString &value)
{
  resourceName = value;
}

const QString &GenericResourceContainer::name() const
{
  return resourceName;
}

Resource *GenericResourceContainer::newResource(const QJsonObject &data)
{
  return new GenericResource(data);
}
