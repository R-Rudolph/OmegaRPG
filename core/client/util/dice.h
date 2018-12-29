#ifndef DICE_H
#define DICE_H

#include<QRandomGenerator>

class Dice
{
  static QRandomGenerator _diceRandomGenerator;
public:
  static quint32 roll(int diceValue);
  static void reseed();
};

#endif // DICE_H
