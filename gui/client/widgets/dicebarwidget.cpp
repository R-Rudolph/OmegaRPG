#include "dicebarwidget.h"
#include <QHBoxLayout>
#include <QSpinBox>

void DiceBarWidget::initButton(QPushButton*& button, int value)
{
  button = new QPushButton(this);
  QString valueString = QString::number(value);
  button->setStyleSheet("QPushButton"
                        "{"
                        "color: black invert;"
                        "border-image: url(:/icons/d"+valueString+".png) 0 0 0 0 stretch stretch;"
                        "font-size: 8px;"
                        "margin-right: 3px;"
                        "}");
  button->setText(valueString);
  button->setToolTip("d"+valueString);
  button->setFlat(true);
  button->setMinimumHeight(20);
  button->setMaximumHeight(20);
  button->setMaximumWidth(20);
  button->setMinimumWidth(20);
  diceMap.insert((QObject*)button,value);
  connect(button,&QPushButton::clicked,this,&DiceBarWidget::diceButtonPressed);
}

DiceBarWidget::DiceBarWidget(QWidget *parent) : QWidget(parent)
{
  QHBoxLayout* layout = new QHBoxLayout(this);
  numDiceBox = new QSpinBox(this);
  numDiceBox->setMinimum(-999);
  numDiceBox->setMaximum(999);
  layout->addWidget(numDiceBox,0,Qt::AlignLeft);
  std::initializer_list<int> diceList = {2,3,4,6,8,10,12,20,100};
  foreach(int value, QVector<int>(diceList))
  {
    QPushButton* diceButton;
    initButton(diceButton,value);
    layout->addWidget(diceButton,0,Qt::AlignLeft);
    dice.append(diceButton);
  }
}

void DiceBarWidget::diceButtonPressed()
{
  if(diceMap.contains(QObject::sender()))
  {
    emit diceRoll(numDiceBox->value(),diceMap[QObject::sender()]);
  }
}
