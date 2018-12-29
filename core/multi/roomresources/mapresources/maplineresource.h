#ifndef MAPLINERESOURCE_H
#define MAPLINERESOURCE_H

#include "../resource.h"
#include<QLineF>
#include<QString>

class MapLineResource : public Resource
{
public:
  static const QString resourceName;
private:
  QLineF line;
  int width;
  QString color;
public:
  MapLineResource();
  MapLineResource(const QJsonObject& data);
  MapLineResource(const QLineF& line, int width=0, const QString& color="");
  virtual QJsonObject data() const;
  QLineF getLine() const;
  void setLine(const QLineF &value);
  int getWidth() const;
  void setWidth(int value);
  QString getColor() const;
  void setColor(const QString &value);
  virtual const QString& name() const;
};

#endif // MAPLINERESOURCE_H
