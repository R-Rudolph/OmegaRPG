#include "servereditdialog.h"

ServerEditDialog::ServerEditDialog() : NightModeDialog()
{
  mainLayout = new QGridLayout(getCentralWidget());
  buttonLayout = new QHBoxLayout;
  nameLabel = new QLabel("Name:",this);
  addressLabel = new QLabel("Address:",this);
  nameEdit = new QLineEdit(this);
  addressEdit = new QLineEdit(this);
  okButton = new QPushButton("Ok",this);
  cancelButton = new QPushButton("Cancel",this);
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);
  mainLayout->addWidget(nameLabel,0,0,1,1);
  mainLayout->addWidget(nameEdit,0,1,1,1);
  mainLayout->addWidget(addressLabel,1,0,1,1);
  mainLayout->addWidget(addressEdit,1,1,1,1);
  mainLayout->addLayout(buttonLayout,2,0,1,2);

  connect(okButton,&QPushButton::clicked,this,&ServerEditDialog::okPressed);
  connect(cancelButton,&QAbstractButton::clicked,this,&QDialog::reject);
}

QString ServerEditDialog::getName() const
{
  return nameEdit->text();
}

void ServerEditDialog::setName(const QString &newName)
{
  nameEdit->setText(newName);
}

QString ServerEditDialog::getAddr() const
{
  return addressEdit->text();
}

void ServerEditDialog::setAddr(const QString &newAddr)
{
  addressEdit->setText(newAddr);
}

void ServerEditDialog::okPressed(bool)
{
  if(addressEdit->text().isEmpty())
  {
    QMessageBox::warning(this,"Invalid Data","The favorite must have an address.",QMessageBox::Ok);
    return;
  }
  else if(nameEdit->text().isEmpty())
  {
    nameEdit->setText(addressEdit->text());
  }
  accept();
}
