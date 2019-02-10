#include "dicerollentry.h"
#include "diceroll.h"
#include "dice.h"

DiceRollEntry::DiceRollEntry()
{
  value_   = 0;
  numDice_ = 0;
}

QString DiceRollEntry::signString(bool signOnPositive) const
{
  return (value_<0) ? "- " : signOnPositive ? "+ " : "";
}

QString DiceRollEntry::nameString() const
{
  if(name_.isEmpty())
    return "";
  else
  {
    return " ("+name_+")";
  }
}

DiceRollEntry::DiceRollEntry(const QRegularExpressionMatch& regexMatch)
  :DiceRollEntry()
{
  name_ = regexMatch.captured(DiceRoll::nameString_);
  QString num = regexMatch.captured(DiceRoll::numString_);
  QString sign = regexMatch.captured(DiceRoll::signString_);
  QString dice = regexMatch.captured(DiceRoll::diceString_);
  QString value = regexMatch.captured(DiceRoll::valueString_);

  value_ = value.toInt();
  if(sign=="-")
    value_ = -value_;
  if(!dice.isEmpty())
  {
    numDice_ = 1;
    if(!num.isEmpty())
    {
      numDice_ = num.toInt();
    }
  }
}

DiceRollResult DiceRollEntry::roll(bool signOnPositive) const
{
  if(numDice_ == 0)
  {
    if(value_ == 0)
      return DiceRollResult("",0);
    else
    {
      QString text = signString(signOnPositive)+QString::number(std::abs(value_))+nameString();
      return DiceRollResult(text,value_);
    }
  }
  else
  {
    QString text = signString(signOnPositive);
    text += ((numDice_==1) ? "" : QString::number(numDice_));
    text += "d"+QString::number(std::abs(value_))+":";
    QStringList rollList;
    rollList.reserve(numDice_);
    int sum = 0;
    for(int i=0;i<numDice_;i++)
    {
      int rolledValue = static_cast<int>(Dice::roll(std::abs(value_)));
      if(value_ < 0)
        rolledValue = - rolledValue;
      rollList.append(QString::number(std::abs(rolledValue)));
      sum += rolledValue;
    }
    text += rollList.join(',');
    text += nameString();
    return DiceRollResult(text,sum);
  }
}
