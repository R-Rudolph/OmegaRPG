#include "dice.h"

QRandomGenerator Dice::_diceRandomGenerator = QRandomGenerator(QRandomGenerator::system()->generate());


quint32 Dice::roll(int diceValue)
{
  return _diceRandomGenerator.bounded(1,diceValue+1);
}

void Dice::reseed()
{
  _diceRandomGenerator.seed(QRandomGenerator::system()->generate());
}
