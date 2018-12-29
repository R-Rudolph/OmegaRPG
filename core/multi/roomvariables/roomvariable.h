#ifndef ROOMVARIABLE_H
#define ROOMVARIABLE_H

#include <QJsonValue>
#include "core/multi/util/player.h"

class RoomVariable
{
public:
  RoomVariable();
  virtual ~RoomVariable();
  virtual bool isValid() const;
  virtual QJsonValue data() const =0;
  virtual const QString& name() const = 0;
};

#endif // ROOMVARIABLE_H
