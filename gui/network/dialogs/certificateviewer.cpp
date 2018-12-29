#include "certificateviewer.h"

QLabel *CertificateViewer::label(const QStringList &list)
{
  if(list.size()==0)
    return new QLabel("&lt;Not Part Of Certificate&gt;");
  else
  {
    QString text;
    for(int i=0;i<list.size()-1;i++)
      text.append(list.at(i).toHtmlEscaped()+"<br/>");
    text.append(list[list.size()-1]);
    return new QLabel(text);
  }
}

QString CertificateViewer::hexString(const QByteArray &array, int lineLength)
{
  QStringList list;
  QString hexString = QString::fromLatin1(array);
  for(int i=0;i<hexString.size();i=i+2)
  {
    list.append(hexString.mid(i,2));
  }
  QString result = list.join(":").toUpper();
  if(lineLength!=0)
  {
    QString result2;
    for(int i=0;i<result.length();i+=lineLength)
    {
      result2.append(result.mid(i,lineLength));
      result2.append("<br/>");
    }
    result = result2;
  }
  return "<pre>"+result+"</pre>";
}

CertificateViewer::CertificateViewer(const QSslCertificate &cert, QWidget *parent) : QDialog(parent)
{
  layout = new QFormLayout(this);

  issuedTo = new QLabel("Issued To");
  issuedTo->setStyleSheet("font-weight:bold");
  layout->addRow(issuedTo);
  layout->addRow("Common Name (CN)"        ,label(cert.subjectInfo(QSslCertificate::CommonName)));
  layout->addRow("Organization (O)"        ,label(cert.subjectInfo(QSslCertificate::Organization)));
  layout->addRow("Organizational Unit (OU)",label(cert.subjectInfo(QSslCertificate::OrganizationalUnitName)));
  layout->addRow("Country (C)"             ,label(cert.subjectInfo(QSslCertificate::CountryName)));
  layout->addRow("State (ST)"              ,label(cert.subjectInfo(QSslCertificate::StateOrProvinceName)));
  layout->addRow("Locality (L)"            ,label(cert.subjectInfo(QSslCertificate::LocalityName)));
  layout->addRow(new QLabel());

  issuedBy = new QLabel("Issued By");
  issuedBy->setStyleSheet("font-weight:bold");
  layout->addRow(issuedBy);
  if(!cert.isSelfSigned())
  {
    layout->addRow("Common Name (CN)"        ,label(cert.issuerInfo(QSslCertificate::CommonName)));
    layout->addRow("Organization (O)"        ,label(cert.issuerInfo(QSslCertificate::Organization)));
    layout->addRow("Organizational Unit (OU)",label(cert.issuerInfo(QSslCertificate::OrganizationalUnitName)));
    layout->addRow("Country (C)"             ,label(cert.issuerInfo(QSslCertificate::CountryName)));
    layout->addRow("State (ST)"              ,label(cert.issuerInfo(QSslCertificate::StateOrProvinceName)));
    layout->addRow("Locality (L)"            ,label(cert.issuerInfo(QSslCertificate::LocalityName)));
  }
  else
  {
    layout->addRow("Is self-signed",new QWidget());
  }
  layout->addRow(new QLabel());

  periodOfValidity = new QLabel("Period of Validity");
  periodOfValidity->setStyleSheet("font-weight:bold");
  layout->addRow(periodOfValidity);
  layout->addRow("Begins On",new QLabel(cert.effectiveDate().toString()));
  layout->addRow("Expires On",new QLabel(cert.expiryDate().toString()));
  layout->addRow(new QLabel());

  fingerprints = new QLabel("Fingerprints");
  fingerprints->setStyleSheet("font-weight:bold");
  layout->addRow(fingerprints);
  //layout->addRow("SHA-256",);
  layout->addRow("SHA-256",new QLabel(hexString(cert.digest(QCryptographicHash::Sha256).toHex(),48)));
  layout->addRow("SHA1",new QLabel(hexString(cert.digest(QCryptographicHash::Sha1).toHex(),60)));
  layout->addRow(new QLabel());

  closeButton = new QPushButton("Close");
  layout->addRow("",closeButton);

  connect(closeButton,&QPushButton::clicked,this,&CertificateViewer::close);
}

void CertificateViewer::call(const QSslCertificate &certificate, QWidget *parent)
{
  CertificateViewer viewer(certificate,parent);
  viewer.exec();
}
