#ifndef CERTIFICATEGENERATIONDIALOG_H
#define CERTIFICATEGENERATIONDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QSslKey>
#include <QSslCertificate>
#include <QFile>
#include <QMessageBox>

#include "core/multi/util/global.h"
#include "util/sslhelper.h"
#include "widgets/fileselectwidget.h"

class CertificateGenerationDialog : public QDialog
{
  Q_OBJECT
  QFormLayout* layout;
  QHBoxLayout* buttonLayout;
  QLineEdit* countryEdit;
  QLineEdit* organisationEdit;
  QLineEdit* commonNameEdit;
  QLineEdit* organisationUnitEdit;
  QLineEdit* localityEdit;
  QLineEdit* stateEdit;
  QSpinBox* keyLength;
  FileSelectWidget* keyPathEdit;
  FileSelectWidget* certPathEdit;
  QPushButton* okButton;
  QPushButton* cancelButton;

  QSslKey privateKey;
  QSslCertificate certificate;
public:
  explicit CertificateGenerationDialog(QWidget *parent = 0);
  void setPaths(const QString& key, const QString& cert);
  QSslKey getKey();
  QSslCertificate getCertificate();
  QString getKeyPath();
  QString getCertPath();
signals:

private slots:
  void okButtonPressed();
};

#endif // CERTIFICATEGENERATIONDIALOG_H
