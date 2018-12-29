#include "certificatedialog.h"

CertificateDialog::CertificateDialog(const QSslCertificate &cert, const QString &errorMsg, QWidget *parent) : NightModeDialog(parent)
{
  setModal(true);
  setWindowTitle("SSL Error");
  layout = new QVBoxLayout(getCentralWidget());
  buttonLayout = new QHBoxLayout();
  firstRow = new QLabel("SSL Error:");
  secondRow = new QLabel(errorMsg);
  thirdRow = new QLabel("Allow connection anyway?");
  yesPermButton = new QPushButton("Yes (Permanently)");
  yesTempButton = new QPushButton("Yes (Temporary)");
  noButton = new QPushButton("No");
  layout->addWidget(firstRow);
  layout->addWidget(secondRow);
  layout->addWidget(thirdRow);
  layout->addLayout(buttonLayout);
  if(!cert.isNull())
  {
    this->certificate = cert;
    viewCertButton = new QPushButton("View Certificate");
    buttonLayout->addWidget(viewCertButton);
  }
  buttonLayout->addWidget(noButton);
  buttonLayout->addWidget(yesPermButton);
  buttonLayout->addWidget(yesTempButton);

  connect(yesPermButton,&QPushButton::clicked,this,&CertificateDialog::pressedYesPerm);
  connect(yesTempButton,&QPushButton::clicked,this,&CertificateDialog::pressedYesTemp);
  connect(noButton,&QPushButton::clicked,this,&CertificateDialog::pressedNo);
  connect(viewCertButton,&QPushButton::clicked,this,&CertificateDialog::pressedViewCert);
}

void CertificateDialog::pressedYesPerm()
{
  done(YesPermanently);
}

void CertificateDialog::pressedYesTemp()
{
  done(YesTemporary);
}

void CertificateDialog::pressedNo()
{
  done(No);
}

void CertificateDialog::pressedViewCert()
{
  CertificateViewer::call(certificate,this);
}
