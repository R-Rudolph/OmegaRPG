#include "dicerollnotesdialog.h"

DiceRollNotesDialog::DiceRollNotesDialog(QWidget *parent, const QString& text) : NightModeDialog(parent)
{
  layout = new QVBoxLayout(getCentralWidget());
  notesBrowser = new QTextBrowser(this);
  notesBrowser->setPlainText(text);
  okButton = new QPushButton("Ok",this);
  layout->addWidget(notesBrowser);
  layout->addWidget(okButton);
  QSizePolicy pol = okButton->sizePolicy();
  pol.setVerticalPolicy(QSizePolicy::Minimum);
  okButton->setSizePolicy(pol);
  connect(okButton,&QPushButton::clicked,this,&QDialog::reject);
}
