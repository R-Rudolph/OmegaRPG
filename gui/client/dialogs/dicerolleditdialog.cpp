#include "dicerolleditdialog.h"
#include "widgets/dicebarwidget.h"

DiceRollEditDialog::DiceRollEditDialog(QWidget *parent) : NightModeDialog(parent)
{
  mainLayout = new QFormLayout(getCentralWidget());
  buttonLayout = new QHBoxLayout();
  nameEdit = new QLineEdit(this);
  bonusEdit = new QLineEdit(this);
  notesEdit = new QTextEdit(this);
  rollParentBox = new QCheckBox(this);
  okButton = new QPushButton("Ok",this);
  cancelButton = new QPushButton("Cancel",this);
  diceBar = new DiceBarWidget(this);
  diceBar->setMinimumWidth(200);

  rollParentBox->setChecked(true);

  mainLayout->addRow("Name",nameEdit);
  mainLayout->addRow("Add Roll",diceBar);
  mainLayout->addRow("Roll/Bonus",bonusEdit);
  mainLayout->addRow("Roll Parent Dice",rollParentBox);
  mainLayout->addRow("Notes",notesEdit);
  mainLayout->addRow(buttonLayout);
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);

  connect(bonusEdit,&QLineEdit::textChanged,this,&DiceRollEditDialog::bonusChanged);
  connect(okButton,&QPushButton::clicked,this,&QDialog::accept);
  connect(cancelButton,&QPushButton::clicked,this,&QDialog::reject);
  connect(diceBar,&DiceBarWidget::diceRoll,this,&DiceRollEditDialog::addDiceRoll);
}

void DiceRollEditDialog::setName(const QString &name)
{
  nameEdit->setText(name);
}

void DiceRollEditDialog::setBonus(const QString &bonus)
{
  bonusEdit->setText(bonus);
  bonusChanged();
}

void DiceRollEditDialog::setNotes(const QString &notes)
{
  notesEdit->setPlainText(notes);
}

void DiceRollEditDialog::setRollParent(bool value)
{
  rollParentBox->setChecked(value);
}

QString DiceRollEditDialog::getName() const
{
  return nameEdit->text();
}

QString DiceRollEditDialog::getBonus() const
{
  return bonusEdit->text();
}

QString DiceRollEditDialog::getNotes() const
{
  return notesEdit->toPlainText();
}

bool DiceRollEditDialog::getRollParent() const
{
  return rollParentBox->checkState()==Qt::Checked;
}

void DiceRollEditDialog::addDiceRoll(int num, int value)
{
  QString appendString;
  if(num==0)
  {
    appendString = " + "+QString::number(value);
  }
  else if(num<1)
  {
    if(num!=-1)
      appendString = " - "+QString::number(qAbs(num))+"d"+QString::number(value);
    else
      appendString = " - d"+QString::number(value);
  }
  else
    if(num!=1)
      appendString = " + "+QString::number(num)+"d"+QString::number(value);
    else
      appendString = " + d"+QString::number(value);
  if(bonusEdit->text().isEmpty())
  {
    if(num<0)
      appendString.remove(0,1);
    else
      appendString.remove(0,3);
  }
  bonusEdit->setText(bonusEdit->text() + appendString);
}

void DiceRollEditDialog::bonusChanged()
{
  QString bonus = bonusEdit->text();
  if(AliasDiceRoll::matchesExpression(bonus))
  {
    bonusEdit->setToolTip("");
    bonusEdit->setStyleSheet("color: #000000");
  }
  else
  {
    bonusEdit->setToolTip("Incorrect pattern. Try patterns like\nd20 + 1d6 + 4");
    bonusEdit->setStyleSheet("color: #FF0000");
  }
}
