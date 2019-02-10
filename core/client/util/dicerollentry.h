#ifndef DICEROLLENTRY_H
#define DICEROLLENTRY_H

#include<QRegularExpressionMatch>
#include "dicerollresult.h"

class DiceRollEntry
{
  int numDice_;
  int value_;
  QString name_;

  QString signString(bool signOnPositive) const;
  QString nameString() const;
public:
  DiceRollEntry();
  DiceRollEntry(const QRegularExpressionMatch& regexMatch);
  DiceRollResult roll(bool signOnPositive) const;
};

#endif // DICEROLLENTRY_H
