#include "diceroll.h"

const QString DiceRoll::globalNameString_ = "globalName";
const QString DiceRoll::signString_ = "sign";
const QString DiceRoll::numString_ = "num";
const QString DiceRoll::diceString_ = "dice";
const QString DiceRoll::valueString_ = "value";
const QString DiceRoll::nameString_ = "name";
const QString DiceRoll::remainingString_ = "remaining";

const QRegularExpression DiceRoll::completeStringRegEx_ =
      QRegularExpression("^"
                         "([\\s\\t]*(?<"+globalNameString_+">.*?)[\\s\\t]*:)?[\\s\\t]*"                         /// name
                         "(?<"+signString_+">[+-]?)[\\s\\t]*"                                                   /// sign of first roll
                         "(((?<"+numString_+">\\d+)?(?<"+diceString_+">d))?(?<"+valueString_+">\\d+))[\\s\\t]*" /// dice of first roll
                         "(\\((?<"+nameString_+">.*?)\\))?[\\s\\t]*"                                            /// name of first roll
                         "(?<"+remainingString_+">([+-][\\s\\t]*((\\d+)?(d))?(\\d+)[\\s\\t]*(.*?)[\\s\\t]*?)*)"   ///remaining rolls
                         "$");

QString DiceRoll::rawString() const
{
  return rawString_;
}

DiceRoll::DiceRoll()
{
  valid_ = false;
}

DiceRoll::DiceRoll(const QString& diceRollString)
  : DiceRoll()
{
  rawString_ = diceRollString;
  QString currentString = diceRollString;
  auto matches = completeStringRegEx_.match(currentString);
  if(!matches.hasMatch())
  {
    return;
  }
  valid_ = true;
  DiceRollEntry newEntry(matches);
  rolls_.append(newEntry);
  QString globalName = matches.captured(globalNameString_);
  if(!globalName.isEmpty())
    name_ = globalName;
  currentString = matches.captured(remainingString_);

  while(!currentString.isEmpty())
  {
    matches = completeStringRegEx_.match(currentString);
    DiceRollEntry newEntry(matches);
    rolls_.append(newEntry);
    currentString = matches.captured(remainingString_);
  }
}

bool DiceRoll::valid() const
{
  return valid_;
}

QString DiceRoll::roll() const
{
  QString result = name_ + ":";
  int sum = 0;
  for(int i=0;i<rolls_.size();i++)
  {
    auto rollResult = rolls_[i].roll(i!=0);
    sum    += rollResult.sum();
    result += " " + rollResult.text();
  }
  result += " = " + QString::number(sum);
  return result;
}

