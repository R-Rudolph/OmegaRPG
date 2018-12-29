#include "miniaturesizespinbox.h"
#include <QtMath>

MiniatureSizeSpinBox::MiniatureSizeSpinBox(QWidget *parent) : QDoubleSpinBox(parent)
{

}

QString MiniatureSizeSpinBox::textFromValue(double val) const
{
  return QString::number(val);
}

void MiniatureSizeSpinBox::stepBy(int steps)
{
  int stepsabs = qAbs(steps);
  for(int i=0;i<stepsabs;i++)
  {
    qreal previousValue = value();
    if(steps>0)
    {
      if(value()>=1)
        setValue(qFloor(value()+1));
      else
      {
        qreal rectifiedValue = qFloor(qLn(value())/qLn(2));
        setValue(qPow(2,rectifiedValue+1));
        if(value()==previousValue)
        {
          setValue(qPow(2,rectifiedValue+2));
        }
      }
    }
    else
    {
      if(value()>1)
        setValue(qCeil(value()-1));
      else
      {
        qreal rectifiedValue = qCeil(qLn(value())/qLn(2));
        setValue(qPow(2,rectifiedValue-1));
        if(value()==previousValue)
        {
          setValue(qPow(2,rectifiedValue-2));
        }
      }
    }
  }
}
