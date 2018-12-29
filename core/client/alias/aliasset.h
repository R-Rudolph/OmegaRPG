#ifndef ALIASSET_H
#define ALIASSET_H

#include <QUrl>
#include <QVector>
#include <QList>
#include "alias.h"
#include <QJsonArray>


class AliasSet
{
public:
  AliasSet(const QString& name="New Alias Set");
  AliasSet(const QJsonObject&);

  QString name;
  bool selected;
  QVector<Alias> set;
  QVector<AliasSet> subsets;

  QJsonObject toJson() const;
  QList<const Alias*> getSelectedAliases() const;
  AliasSet& addSet(const QString& name="New Alias Set");
  Alias& addAlias(const QString& name="No Name", const QString& color="#000000");
};

#endif // ALIASSET_H
