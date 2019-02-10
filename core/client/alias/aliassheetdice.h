#ifndef ALIASSHEETDICE_H
#define ALIASSHEETDICE_H

#include <QJsonObject>
#include <QList>
#include <QJsonArray>
#include "aliassheet.h"
#include "aliasdiceroll.h"

class AliasDiceRoll;

class AliasSheetDice : public AliasSheet
{
  AliasDiceRoll rolls;
public:
  const static QString className;
  AliasSheetDice();
  AliasSheetDice(AliasSheetDice& other);
  AliasSheetDice(const QJsonObject& json);
  virtual QJsonObject toJson() const;
  virtual void loadJson(const QJsonObject& sheetObject);
  virtual AliasSheet* copy() const;
  AliasDiceRoll getRolls() const;
  void setRolls(const AliasDiceRoll &value);
  virtual SheetType getType() const;
};

#endif // ALIASSHEETDICE_H
