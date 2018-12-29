#ifndef CERTIFICATEDIALOG_H
#define CERTIFICATEDIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSslCertificate>
#include <QLabel>
#include <QMessageBox>
#include "dialogs/certificateviewer.h"
#include "dialogs/nightmodedialog.h"

class CertificateDialog : public NightModeDialog
{
  Q_OBJECT
  QSslCertificate certificate;
  QVBoxLayout* layout;
  QHBoxLayout* buttonLayout;
  QLabel* firstRow;
  QLabel* secondRow;
  QLabel* thirdRow;
  QPushButton* yesPermButton;
  QPushButton* yesTempButton;
  QPushButton* noButton;
  QPushButton* viewCertButton;
public:
  enum DialogResults
  {
    No,
    YesTemporary,
    YesPermanently
  };
  explicit CertificateDialog(const QSslCertificate& cert, const QString& errorMsg, QWidget *parent = 0);

signals:

public slots:

private slots:
  void pressedYesPerm();
  void pressedYesTemp();
  void pressedNo();
  void pressedViewCert();
};

#endif // CERTIFICATEDIALOG_H
