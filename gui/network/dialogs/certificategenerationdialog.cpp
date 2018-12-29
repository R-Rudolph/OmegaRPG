#include "certificategenerationdialog.h"
#include<QLabel>

CertificateGenerationDialog::CertificateGenerationDialog(QWidget *parent) : QDialog(parent)
{
  setModal(true);
  layout = new QFormLayout(this);
  buttonLayout = new QHBoxLayout();
  //init widgets
  countryEdit = new QLineEdit();
  organisationEdit = new QLineEdit();
  commonNameEdit = new QLineEdit();
  //keyLength = new QSpinBox();
  keyPathEdit = new FileSelectWidget(false);
  certPathEdit = new FileSelectWidget(false);
  organisationUnitEdit = new QLineEdit();
  localityEdit = new QLineEdit();
  stateEdit = new QLineEdit();
  okButton = new QPushButton("Generate");
  cancelButton = new QPushButton("Cancel");
  //init values
  countryEdit->setPlaceholderText("US");
  countryEdit->setToolTip("The abbreviation of your country (e.g. US, DE, etc.)");
  organisationEdit->setToolTip("Your organization (if any)");
  commonNameEdit->setToolTip("The common name (or domain name) for this certificate. (e.g. example.org, *.example.org or something else, if you have no domain. You know, whatever.");
  commonNameEdit->setPlaceholderText("example.org");
  /*
  keyLength->setMinimum(1024);
  keyLength->setMaximum(1048576);
  keyLength->setValue(4096);
  keyLength->setToolTip("The length of the RSA key generated (in bits)");
  */
  //layout
  layout->addRow("Common Name",commonNameEdit);
  layout->addRow("Country",countryEdit);
  layout->addRow("State/Province",stateEdit);
  layout->addRow("City/Locality",localityEdit);
  layout->addRow("Organization",organisationEdit);
  layout->addRow("Organizational Unit",organisationUnitEdit);
  //layout->addRow("Key Length",keyLength);
  layout->addRow("Private Key Path",keyPathEdit);
  layout->addRow("Certificate Path",certPathEdit);
  layout->addRow(buttonLayout);
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);
  //signals
  connect(okButton,&QPushButton::clicked,this,&CertificateGenerationDialog::okButtonPressed);
  connect(cancelButton,&QPushButton::clicked,this,&CertificateGenerationDialog::reject);
}

void CertificateGenerationDialog::setPaths(const QString &key, const QString &cert)
{
  keyPathEdit->setText(key);
  certPathEdit->setText(cert);
}

QSslKey CertificateGenerationDialog::getKey()
{
  return privateKey;
}

QSslCertificate CertificateGenerationDialog::getCertificate()
{
  return certificate;
}

QString CertificateGenerationDialog::getKeyPath()
{
  return keyPathEdit->text();
}

QString CertificateGenerationDialog::getCertPath()
{
  return certPathEdit->text();
}

void CertificateGenerationDialog::okButtonPressed()
{
  QString files;
  if(QFile(keyPathEdit->text()).exists())
    files.append(keyPathEdit->text()+"\n");
  if(QFile(certPathEdit->text()).exists())
    files.append(certPathEdit->text()+"\n");
  if(!files.isEmpty())
  {
    if(QMessageBox::question(this,this->windowTitle(),"These file(s):\n"+files+"already exist. Overwrite?",QMessageBox::Yes|QMessageBox::No)!=QMessageBox::Yes)
    {
      return;
    }
  }
  SslHelper::GenerationError error = SslHelper::generateCredentials(certificate,
                                                                    privateKey,
                                                                    countryEdit->text(),
                                                                    organisationEdit->text(),
                                                                    commonNameEdit->text(),
                                                                    organisationUnitEdit->text(),
                                                                    localityEdit->text(),
                                                                    stateEdit->text());
  if(error==SslHelper::ErrorNone)
  {
    if(!SslHelper::saveCertificate(certificate,certPathEdit->text()) ||
       !SslHelper::saveKey(privateKey,keyPathEdit->text()))
    {
      QMessageBox::warning(this,this->windowTitle(),"Could not save key or certificate.",QMessageBox::Ok);
      return;
    }
  }
  else
  {
    QMessageBox::warning(this,this->windowTitle(),"Error during generation. Aborting.",QMessageBox::Ok);
    return;
  }
  accept();
}
