#include "genericresource.h"

GenericResource::GenericResource()
  : Resource()
{

}

GenericResource::GenericResource(const QJsonObject& data)
  : Resource()
{
  this->_jsonData = data;
}

GenericResource::GenericResource(const Resource &resource)
{
  setID(resource.getID());
  _jsonData = resource.data();
  _name = resource.name();
  valid = resource.isValid();
}

QJsonObject GenericResource::data() const
{
  return _jsonData;
}

const QString&GenericResource::name() const
{
  return _name;
}
