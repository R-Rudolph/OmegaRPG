#include "dice.h"
#include<sys/time.h>

#if QT_VERSION >= 0x051000
QRandomGenerator Dice::_diceRandomGenerator = QRandomGenerator(QRandomGenerator::system()->generate());


quint32 Dice::roll(int diceValue)
{
  return _diceRandomGenerator.bounded(1,diceValue+1);
}

void Dice::reseed()
{
  _diceRandomGenerator.seed(QRandomGenerator::system()->generate());
}
#else
bool Dice::seeded_ = false;

quint32 Dice::roll(int diceValue)
{
  if(!seeded_)
  {
    reseed();
  }
  return (qrand()%diceValue)+1;
}

void Dice::reseed()
{
  struct timeval time;
  gettimeofday(&time,NULL);
  qsrand((time.tv_sec * 1000) + (time.tv_usec / 1000));
  seeded_ = true;
}
#endif
