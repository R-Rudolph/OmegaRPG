#ifndef DICE_H
#define DICE_H

#if QT_VERSION >= 0x051000
#include<QRandomGenerator>

class Dice
{
  static QRandomGenerator _diceRandomGenerator;
public:
  static quint32 roll(int diceValue);
  static void reseed();
};
#else
#include<QtGlobal>

class Dice
{
  static bool seeded_;
public:
  static quint32 roll(int diceValue);
  static void reseed();
};
#endif


#endif // DICE_H
