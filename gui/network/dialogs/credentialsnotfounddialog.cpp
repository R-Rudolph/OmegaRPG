#include "credentialsnotfounddialog.h"

CredentialsNotFoundDialog::DialogValue CredentialsNotFoundDialog::getValue() const
{
  return value;
}

CredentialsNotFoundDialog::CredentialsNotFoundDialog(QWidget *parent) : QDialog(parent)
{
  _verticalLayout = new QVBoxLayout();
  _buttonLayout = new QHBoxLayout();

  _text = new QLabel("The certificate and/or the private key could not be found.\n"
                     "Do you want to generate a new certificate, use a temporary certificate, or cancel starting the server?");
  _certificateButton = new QPushButton("New Certificate");
  _temporaryCertificateButton = new QPushButton("Temporary Certificate");
  _cancelButton = new QPushButton("Cancel");
  setLayout(_verticalLayout);
  _verticalLayout->addWidget(_text);
  _verticalLayout->addLayout(_buttonLayout);
  _buttonLayout->addWidget(_certificateButton);
  _buttonLayout->addWidget(_temporaryCertificateButton);
  _buttonLayout->addWidget(_cancelButton);
  value = Cancel;

  connect(_certificateButton,&QPushButton::clicked,this,&CredentialsNotFoundDialog::newCertificateClicked);
  connect(_temporaryCertificateButton,&QPushButton::clicked,this,&CredentialsNotFoundDialog::temporaryCertificateClicked);
  connect(_cancelButton,&QPushButton::clicked,this,&CredentialsNotFoundDialog::cancelClicked);
}

void CredentialsNotFoundDialog::newCertificateClicked()
{
  value = NewCertificate;
  accept();
}

void CredentialsNotFoundDialog::temporaryCertificateClicked()
{
  value = TemporaryCertificate;
  accept();
}

void CredentialsNotFoundDialog::cancelClicked()
{
  value = Cancel;
  accept();
}
