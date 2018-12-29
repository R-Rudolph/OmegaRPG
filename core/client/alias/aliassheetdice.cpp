#include "aliassheetdice.h"

const QString AliasSheetDice::className = "dice";

DiceRoll AliasSheetDice::getRolls() const
{
  return rolls;
}

void AliasSheetDice::setRolls(const DiceRoll &value)
{
  rolls = value;
}

AliasSheet::SheetType AliasSheetDice::getType() const
{
  return SHEET_DICE;
}

AliasSheetDice::AliasSheetDice()
{

}

AliasSheetDice::AliasSheetDice(AliasSheetDice &other)
{
  this->sheetName = other.sheetName;
  this->rolls = other.rolls;
}

AliasSheetDice::AliasSheetDice(const QJsonObject &json)
{
  loadJson(json);
}

void AliasSheetDice::loadJson(const QJsonObject &sheetObject)
{
  this->sheetName = sheetObject["name"].toString();
  this->rolls = DiceRoll(sheetObject["rolls"].toObject());
}

QJsonObject AliasSheetDice::toJson() const
{
  QJsonObject result;
  result.insert("type",className);
  result.insert("name",sheetName);
  result.insert("rolls",rolls.toJson());
  return result;
}

AliasSheet *AliasSheetDice::copy() const
{
  AliasSheetDice* sheet = new AliasSheetDice();
  sheet->sheetName = sheetName;
  sheet->rolls = rolls;
  return sheet;
}
