#ifndef CREDENTIALSNOTFOUNDDIALOG_H
#define CREDENTIALSNOTFOUNDDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

class CredentialsNotFoundDialog : public QDialog
{
  Q_OBJECT
public:
  enum DialogValue
  {
    Cancel,
    NewCertificate,
    TemporaryCertificate
  };
private:
  DialogValue value;
  QVBoxLayout* _verticalLayout;
  QLabel* _text;
  QHBoxLayout* _buttonLayout;
  QPushButton* _cancelButton, * _certificateButton, * _temporaryCertificateButton;
public:
  explicit CredentialsNotFoundDialog(QWidget *parent = nullptr);
  DialogValue getValue() const;
signals:
public slots:
  void newCertificateClicked();
  void temporaryCertificateClicked();
  void cancelClicked();
};

#endif // CREDENTIALSNOTFOUNDDIALOG_H
