#include "knowncertificatesdialog.h"
#include "globalgui.h"

inline bool operator<(const QSslCertificate &e1, const QSslCertificate &e2)
{
  return e1.digest()<e2.digest();
}

uint qHash(const QSslCertificate& cert)
{
  return qHash(cert.digest());
}

QTreeWidgetItem *KnownCertificatesWindow::getPermItem(const QSslCertificate &cert)
{
  for(QMap<QTreeWidgetItem*,QSslCertificate>::Iterator it=permCertificateMap.begin();it!=permCertificateMap.end();it++)
  {
    if(it.value()==cert)
      return it.key();
  }
  return NULL;
}

QTreeWidgetItem *KnownCertificatesWindow::getTempItem(const QSslCertificate &cert)
{
  for(QMap<QTreeWidgetItem*,QSslCertificate>::Iterator it=tempCertificateMap.begin();it!=tempCertificateMap.end();it++)
  {
    if(it.value()==cert)
      return it.key();
  }
  return NULL;
}

void KnownCertificatesWindow::updateCurrentErrors()
{
  errorList->clear();
  errorMap.clear();
  QTreeWidgetItem* item = certificateList->currentItem();
  if(item==NULL)
    return;
  QSslCertificate cert;
  bool found = false;
  bool perm = false;
  if(tempCertificateMap.contains(item))
  {
    found = true;
    cert = tempCertificateMap[item];
  }
  if(!found && permCertificateMap.contains(item))
  {
    perm = true;
    found = true;
    cert = permCertificateMap[item];
  }
  if(found)
  {
    QList<QSslError::SslError> list = tempCertErrorMap.values();
    if(perm)
      list = permCertErrorMap.values();
    foreach(QSslError::SslError error, list)
    {
      QTreeWidgetItem* item = new QTreeWidgetItem(QStringList{getDescriptiveText(QSslError(error))});
      errorList->addTopLevelItem(item);
      errorMap.insert(item,error);
    }
  }
}

QStringList KnownCertificatesWindow::getDescriptiveText(const QSslCertificate &cert)
{
  return cert.subjectInfo(QSslCertificate::CommonName);
}

QString KnownCertificatesWindow::getDescriptiveText(const QSslError &error)
{
  switch(error.error())
  {
    case QSslError::NoError:
      return "No Error";
    case QSslError::UnableToGetIssuerCertificate:
      return "Unable To Get Issuer Certificate";
    case QSslError::UnableToDecryptCertificateSignature:
      return "Unable To Decrypt Certificate Signature";
    case QSslError::UnableToDecodeIssuerPublicKey:
      return "Unable To Decode Issuer Public Key";
    case QSslError::CertificateSignatureFailed:
      return "Certificate Signature Failed";
    case QSslError::CertificateNotYetValid:
      return "Certificate Not Yet Valid";
    case QSslError::CertificateExpired:
      return "Certificate Expired";
    case QSslError::InvalidNotBeforeField:
      return "Invalid Not Before Field";
    case QSslError::InvalidNotAfterField:
      return "Invalid Not After Field";
    case QSslError::SelfSignedCertificate:
      return "Self-Signed Certificate";
    case QSslError::SelfSignedCertificateInChain:
      return "Self-Signed Certificate In Chain";
    case QSslError::UnableToGetLocalIssuerCertificate:
      return "Unable To Get Local Issuer Certificate";
    case QSslError::UnableToVerifyFirstCertificate:
      return "Unable To Verify First Certificate";
    case QSslError::CertificateRevoked:
      return "Certificate Revoked";
    case QSslError::InvalidCaCertificate:
      return "Invalid CA Certificate";
    case QSslError::PathLengthExceeded:
      return "Path Length Exceeded";
    case QSslError::InvalidPurpose:
      return "Invalid Purpose";
    case QSslError::CertificateUntrusted:
      return "Certificate Untrusted";
    case QSslError::CertificateRejected:
      return "Certificate Rejected";
    case QSslError::SubjectIssuerMismatch:
      return "Subject Issuer Mismatch";
    case QSslError::AuthorityIssuerSerialNumberMismatch:
      return "Authority Issuer Serial Number Mismatch";
    case QSslError::NoPeerCertificate:
      return "No Peer Certificate";
    case QSslError::HostNameMismatch:
      return "Host Name Mismatch";
    case QSslError::NoSslSupport:
      return "No Ssl Support";
    case QSslError::CertificateBlacklisted:
      return "Certificate Blacklisted";
    default:
      return "Unspecified Error";
  }
}

QSslCertificate KnownCertificatesWindow::getCertificate(QTreeWidgetItem *item)
{
  if(tempCertificateMap.contains(item))
    return tempCertificateMap[item];
  else if(permCertificateMap.contains(item))
    return permCertificateMap[item];
  return QSslCertificate();
}

KnownCertificatesWindow::KnownCertificatesWindow(QWidget *parent) : QWidget(parent,Qt::Window)
{
  setWindowTitle("OmegaRPG SSL Exceptions");
  ssl = SslSettings::get();
  mainLayout = new QHBoxLayout(this);
  certificateList = new QTreeWidget();
  rightLayout = new QVBoxLayout();
  mainLayout->addWidget(certificateList);
  mainLayout->addLayout(rightLayout);
  errorList = new QTreeWidget();
  closeButton = new QPushButton("Close");
  rightLayout->addWidget(errorList,1);
  rightLayout->addWidget(closeButton,0);

  certificateList->setHeaderLabel("Certificates");
  errorList->setHeaderLabel("Allowed Errors");
  errorList->setRootIsDecorated(false);

  certificateList->setContextMenuPolicy(Qt::CustomContextMenu);
  errorList->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(closeButton,&QPushButton::clicked,this,&KnownCertificatesWindow::close);
  connect(certificateList,&QTreeWidget::customContextMenuRequested,this,&KnownCertificatesWindow::certificatesContextMenu);
  connect(errorList,&QTreeWidget::customContextMenuRequested,this,&KnownCertificatesWindow::errorContextMenu);


  permCertItem = new QTreeWidgetItem(QStringList({"Permanent"}));
  tempCertItem = new QTreeWidgetItem(QStringList({"Temporary"}));
  permCertItem->setIcon(0,GlobalGUI::freedesktopFolder());
  tempCertItem->setIcon(0,GlobalGUI::freedesktopFolder());

  certificateList->addTopLevelItem(permCertItem);
  certificateList->addTopLevelItem(tempCertItem);
  permCertItem->setExpanded(true);
  tempCertItem->setExpanded(true);

  connect(ssl,&SslSettings::permanentExceptionAdded,this,&KnownCertificatesWindow::permanentExceptionAdded);
  connect(ssl,&SslSettings::permanentExceptionRemoved,this,&KnownCertificatesWindow::permanentExceptionRemoved);
  connect(ssl,&SslSettings::temporaryExceptionAdded,this,&KnownCertificatesWindow::temporaryExceptionAdded);
  connect(ssl,&SslSettings::temporaryExceptionRemoved,this,&KnownCertificatesWindow::temporaryExceptionRemoved);
  connect(certificateList,&QTreeWidget::currentItemChanged,this,&KnownCertificatesWindow::updateCurrentErrors);

  foreach(QSslError error, ssl->getPermanentExceptions())
  {
    permanentExceptionAdded(error);
  }
  foreach(QSslError error, ssl->getTemporaryExceptions())
  {
    temporaryExceptionAdded(error);
  }
}

bool KnownCertificatesWindow::restoreGeometry(const QByteArray &geometry)
{
  bool result = QWidget::restoreGeometry(geometry);
  return result;
}

void KnownCertificatesWindow::closeEvent(QCloseEvent *event)
{
  lastSize = QWidget::size();
  lastPosition = QWidget::pos();
  event->accept();
}

void KnownCertificatesWindow::show()
{
  QWidget::show();
  if(lastSize.isValid())
  {
    resize(QSize(0,0));
    move(lastPosition);
    resize(lastSize);
  }
}

void KnownCertificatesWindow::certificatesContextMenu(const QPoint &pos)
{
  QTreeWidgetItem* item = certificateList->itemAt(pos);
  if(item==NULL || item== permCertItem || item==tempCertItem)
    return;
  currentCertItem = item;
  QMenu menu(this);
  menu.addAction("View Certificate",this,&KnownCertificatesWindow::viewCertificate);
  menu.addSeparator();
  menu.addAction("Remove",this,&KnownCertificatesWindow::removeCertificate);
  menu.exec(certificateList->viewport()->mapToGlobal(pos));
}

void KnownCertificatesWindow::errorContextMenu(const QPoint &pos)
{
  QTreeWidgetItem* item = certificateList->itemAt(pos);
  if(item==NULL)
    return;
  currentCertItem = certificateList->currentItem();
  currentErrorItem = item;
  QMenu menu(this);
  menu.addAction("Remove",this,&KnownCertificatesWindow::removeError);
  menu.exec(errorList->viewport()->mapToGlobal(pos));
}

void KnownCertificatesWindow::viewCertificate()
{
  QSslCertificate cert = getCertificate(currentCertItem);
  if(!cert.isNull())
  {
    CertificateViewer viewer(cert);
    viewer.exec();
  }
}

void KnownCertificatesWindow::removeCertificate()
{
  QTreeWidgetItem* item = currentCertItem;
  QMessageBox::StandardButton reply =
      QMessageBox::question(this, "Remove Exceptions", "Are you sure you want to remove all exceptions for this certificate?", QMessageBox::Yes|QMessageBox::No);
  if(reply == QMessageBox::Yes)
  {
    if(tempCertificateMap.contains(item))
    {
      QSslCertificate cert = tempCertificateMap[item];
      foreach(QSslError::SslError error, tempCertErrorMap.values(cert))
      {
        ssl->removeTemporaryException(QSslError(error,cert));
      }
    }
    else if(permCertificateMap.contains(item))
    {
      QSslCertificate cert = permCertificateMap[item];
      foreach(QSslError::SslError error, permCertErrorMap.values(cert))
      {
        ssl->removePermanentException(QSslError(error,cert));
      }
    }
  }
}

void KnownCertificatesWindow::removeError()
{
  QSslCertificate cert;
  bool permanent = false;
  QSslError::SslError error = errorMap[currentErrorItem];
  if(tempCertificateMap.contains(currentCertItem))
  {
    cert = tempCertificateMap[currentCertItem];
  }
  else if(permCertificateMap.contains(currentCertItem))
  {
    cert = permCertificateMap[currentCertItem];
    permanent = true;
  }
  if(!cert.isNull())
  {
    QSslError sslError(error,cert);
    QMessageBox::StandardButton reply =
        QMessageBox::question(this, "Remove Exceptions", "Are you sure you want to remove this exception?", QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
      if(permanent)
        ssl->removePermanentException(sslError);
      else
        ssl->removeTemporaryException(sslError);
    }
  }
}

void KnownCertificatesWindow::permanentExceptionAdded(const QSslError &error)
{
  permCertErrorMap.insert(error.certificate(),error.error());
  QTreeWidgetItem* item = getPermItem(error.certificate());
  if(item==NULL)
  {
    item = new QTreeWidgetItem(QStringList{getDescriptiveText(error.certificate())});
    permCertificateMap.insert(item,error.certificate());
    permCertItem->addChild(item);
  }
  else
  {
    if(certificateList->currentItem()==item)
      updateCurrentErrors();
  }
}

void KnownCertificatesWindow::permanentExceptionRemoved(const QSslError &error)
{
  permCertErrorMap.remove(error.certificate(),error.error());
  //permCertErrorMap.remove(error.certificate());
  QTreeWidgetItem* item = getPermItem(error.certificate());
  if(permCertErrorMap.values(error.certificate()).size()==0)
  {
    permCertificateMap.remove(item);
    delete item;
    updateCurrentErrors();
  }
  else
  {
    if(certificateList->currentItem()==item)
    {
      updateCurrentErrors();
    }
  }
}

void KnownCertificatesWindow::temporaryExceptionAdded(const QSslError &error)
{
  tempCertErrorMap.insert(error.certificate(),error.error());
  QTreeWidgetItem* item = getTempItem(error.certificate());
  if(item==NULL)
  {
    item = new QTreeWidgetItem(QStringList{getDescriptiveText(error.certificate())});
    tempCertificateMap.insert(item,error.certificate());
    tempCertItem->addChild(item);
  }
  else
  {
    if(certificateList->currentItem()==item)
      updateCurrentErrors();
  }
}

void KnownCertificatesWindow::temporaryExceptionRemoved(const QSslError &error)
{
  tempCertErrorMap.remove(error.certificate(),error.error());
  //tempCertErrorMap.remove(error.certificate());
  QTreeWidgetItem* item = getTempItem(error.certificate());
  if(tempCertErrorMap.values(error.certificate()).size()==0)
  {
    tempCertificateMap.remove(item);
    delete item;
    updateCurrentErrors();
  }
  else
  {
    if(certificateList->currentItem()==item)
    {
      updateCurrentErrors();
    }
  }
}
