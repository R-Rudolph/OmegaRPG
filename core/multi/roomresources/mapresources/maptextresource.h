#ifndef MAPTEXTRESOURCE_H
#define MAPTEXTRESOURCE_H

#include "../resource.h"
#include<QPointF>
#include<QJsonObject>

class MapTextResource : public Resource
{
public:
  static const QString resourceName;
private:
  QString text;
  QString color;
  int size;
  QPointF pos;
  void updateValidity();
public:
  MapTextResource(const QString& text="", const QString& color="", int size=10, const QPointF& pos=QPointF(0,0));
  MapTextResource(const QJsonObject& data);
  virtual QJsonObject data() const;
  QString getText() const;
  void setText(const QString &value);
  int getSize() const;
  void setSize(int value);
  QPointF getPos() const;
  void setPos(const QPointF &value);
  QString getColor() const;
  void setColor(const QString &value);
  virtual const QString& name() const;
};

#endif // MAPTEXTRESOURCE_H
