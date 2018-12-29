#ifndef CERTIFICATEVIEWER_H
#define CERTIFICATEVIEWER_H

#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QSslCertificate>
#include <QPushButton>
#include <QString>



class CertificateViewer : public QDialog
{
  Q_OBJECT
  QFormLayout* layout;
  QLabel* issuedTo;
  QLabel* issuedBy;
  QLabel* periodOfValidity;
  QLabel* fingerprints;
  QPushButton* closeButton;
  QLabel* label(const QStringList& list);
  QString hexString(const QByteArray& array, int lineLength=0);
public:
  explicit CertificateViewer(const QSslCertificate& certificate, QWidget *parent = 0);
  static void call(const QSslCertificate& certificate, QWidget *parent = 0);
signals:

private slots:

};

#endif // CERTIFICATEVIEWER_H
