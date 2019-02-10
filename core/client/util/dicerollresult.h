#ifndef DICEROLLRESULT_H
#define DICEROLLRESULT_H

#include<QString>

class DiceRollResult
{
  QString text_;
  int sum_;
public:
  DiceRollResult(const QString& text, int sum);
  QString text() const;
  void setText(const QString& text);
  int sum() const;
  void setSum(int sum);
  DiceRollResult operator+(const DiceRollResult& other);
};

#endif // DICEROLLRESULT_H
