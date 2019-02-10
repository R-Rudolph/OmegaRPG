#include "dicerollresult.h"

int DiceRollResult::sum() const
{
  return sum_;
}

void DiceRollResult::setSum(int sum)
{
  sum_ = sum;
}

DiceRollResult DiceRollResult::operator+(const DiceRollResult& other)
{
  return DiceRollResult(this->text_ + " " + other.text_, this->sum_ + other.sum_);
}

DiceRollResult::DiceRollResult(const QString& text, int sum)
{
  text_ = text;
  sum_  = sum;
}

QString DiceRollResult::text() const
{
  return text_;
}

void DiceRollResult::setText(const QString& text)
{
  text_ = text;
}
