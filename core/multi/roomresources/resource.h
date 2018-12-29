#ifndef RESOURCE_H
#define RESOURCE_H

#include <QJsonObject>

class Resource
{
protected:
  int id;
  bool valid;
protected:
  Resource();
public:
  virtual ~Resource();
  int getID() const;
  void setID(int value);
  bool isValid() const;
  virtual QJsonObject data() const = 0;
  virtual const QString& name() const = 0;
};

#endif // RESOURCE_H
