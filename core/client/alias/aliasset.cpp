#include "aliasset.h"

AliasSet::AliasSet(const QString& name)
{
  this->name = name;
  selected = false;
}

QJsonObject AliasSet::toJson() const
{
  QJsonObject json;
  QJsonArray arr;
  QJsonArray arrSubsets;
  for(int i=0;i<set.size();i++)
  {
    arr.push_back(set.at(i).toJson());
  }
  for(int i=0;i<subsets.size();i++)
  {
    arrSubsets.push_back(subsets.at(i).toJson());
  }
  json.insert("aliases",arr);
  json.insert("subsets",arrSubsets);
  json.insert("name",name);
  json.insert("selected",QJsonValue(selected));
  return json;
}

QList<const Alias *> AliasSet::getSelectedAliases() const
{
  QList<const Alias *> list;
  foreach(const Alias &alias, set)
  {
    if(alias.getSelected())
      list.append((const Alias*)&alias);
  }
  foreach(const AliasSet& set,subsets)
  {
    list.append(set.getSelectedAliases());
  }
  return list;
}

AliasSet &AliasSet::addSet(const QString &name)
{
  subsets.push_back(AliasSet(name));
  return subsets[subsets.size()-1];
}

Alias &AliasSet::addAlias(const QString &name, const QString &color)
{
  set.push_back(Alias(name,color,false));
  return set[set.size()-1];
}

AliasSet::AliasSet(const QJsonObject& json)
{
  selected = false;
  name = "Unnamed Set";
  if(json["name"].isString())
    name = json["name"].toString();
  if(json["selected"].isBool())
    selected = json["selected"].toBool();
  if(json["aliases"].isArray())
  {
    QJsonArray array = json["aliases"].toArray();
    for(int i=0;i<array.size();i++)
    {
      if(array.at(i).isObject())
      {
        Alias newAlias(array.at(i).toObject());
        if(!newAlias.getName().isEmpty())
          set.push_back(newAlias);
      }
    }
  }
  if(json["subsets"].isArray())
  {
    QJsonArray array = json["subsets"].toArray();
    for(int i=0;i<array.size();i++)
    {
      if(array.at(i).isObject())
      {
        AliasSet newAliasSet(array.at(i).toObject());
        subsets.push_back(newAliasSet);
      }
    }
  }
}
