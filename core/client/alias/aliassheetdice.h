#ifndef ALIASSHEETDICE_H
#define ALIASSHEETDICE_H

#include <QJsonObject>
#include <QList>
#include <QJsonArray>
#include "aliassheet.h"
#include "diceroll.h"

class DiceRoll;

class AliasSheetDice : public AliasSheet
{
  DiceRoll rolls;
public:
  const static QString className;
  AliasSheetDice();
  AliasSheetDice(AliasSheetDice& other);
  AliasSheetDice(const QJsonObject& json);
  virtual QJsonObject toJson() const;
  virtual void loadJson(const QJsonObject& sheetObject);
  virtual AliasSheet* copy() const;
  DiceRoll getRolls() const;
  void setRolls(const DiceRoll &value);
  virtual SheetType getType() const;
};

#endif // ALIASSHEETDICE_H
