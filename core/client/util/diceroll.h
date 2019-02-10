#ifndef DICEROLL_H
#define DICEROLL_H

#include<QRegularExpression>
#include<QString>
#include<QVector>
#include "core/client/util/dicerollentry.h"

class DiceRoll
{
public:
  static const QRegularExpression completeStringRegEx_;
  static const QString globalNameString_;
  static const QString signString_;
  static const QString numString_;
  static const QString diceString_;
  static const QString valueString_;
  static const QString nameString_;
  static const QString remainingString_;
private:
  bool valid_;
  QString name_;
  QVector<DiceRollEntry> rolls_;
public:
  DiceRoll();
  DiceRoll(const QString& diceRollString);

  bool valid() const;
  DiceRoll operator+(const DiceRoll& other);
  QString roll() const;
};

#endif // DICEROLL_H
