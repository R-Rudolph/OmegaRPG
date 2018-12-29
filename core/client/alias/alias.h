#ifndef ALIAS_H
#define ALIAS_H

#include <QVariant>
#include <QJsonObject>
#include <QString>
#include "core/client/alias/aliassheetbinder.h"

class Alias
{
  QString name;
  QString color;
  bool selected;
  AliasSheetBinder* binder;
public:
  Alias(const QString& name="No Name", const QString& color="#000080", const bool& selected=false);
  Alias(const QJsonObject& json);
  Alias(const Alias& other);
  ~Alias();
  QJsonObject toJson() const;
  QVariant toVariant();
  bool operator ==(const Alias& other);
  Alias& operator =(const Alias& other);
  QString getName() const;
  void setName(const QString &value);
  QString getColor() const;
  void setColor(const QString &value);
  bool getSelected() const;
  void setSelected(bool value);
  AliasSheetBinder *getBinder() const;
  void setBinder(AliasSheetBinder *value);
};

#endif // ALIAS_H
