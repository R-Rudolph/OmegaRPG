#include "maptextdialog.h"

MapTextDialog::MapTextDialog(QWidget *parent) : NightModeDialog(parent)
{
  this->setModal(true);
  layout = new QFormLayout(getCentralWidget());
  buttonLayout = new QHBoxLayout();
  textEdit = new QLineEdit(this);
  sizeBox = new QSpinBox(this);
  okButton = new QPushButton("Ok",this);
  cancelButton = new QPushButton("Cancel",this);
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);
  layout->addRow("Text:",textEdit);
  layout->addRow("Size:",sizeBox);
  layout->addRow(buttonLayout);

  sizeBox->setMinimum(1);
  sizeBox->setMaximum(100);
  sizeBox->setValue(10);

  connect(okButton,&QPushButton::clicked,this,&MapTextDialog::okClicked);
  connect(cancelButton,&QPushButton::clicked,this,&QDialog::reject);
}

QString MapTextDialog::getText()
{
  return textEdit->text();
}

int MapTextDialog::getSize()
{
  return sizeBox->value();
}

void MapTextDialog::setText(const QString &text)
{
  textEdit->setText(text);
}

void MapTextDialog::setSize(int size)
{
  sizeBox->setValue(size);
}

void MapTextDialog::okClicked()
{
  if(textEdit->text().isEmpty())
  {
    reject();
  }
  else
  {
    accept();
  }
}
