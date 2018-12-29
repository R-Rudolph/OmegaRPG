#ifndef DICEBARWIDGET_H
#define DICEBARWIDGET_H

#include <QWidget>
#include <QMap>
#include <QPushButton>

class QPushButton;
class QSpinBox;

class DiceBarWidget : public QWidget
{
  Q_OBJECT
  QMap<QObject*,int> diceMap;
  QList<QPushButton*> dice;
  QSpinBox* numDiceBox;
  void initButton(QPushButton*& button, int value);
public:
  explicit DiceBarWidget(QWidget *parent = 0);
signals:
  void diceRoll(int num, int value);
private slots:
  void diceButtonPressed();
};

#endif // DICEBARWIDGET_H
