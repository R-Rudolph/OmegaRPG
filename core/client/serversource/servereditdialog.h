#ifndef SERVEREDITDIALOG_H
#define SERVEREDITDIALOG_H

#include <QObject>
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

#include "gui/util/globalgui.h"
#include "dialogs/nightmodedialog.h"

class ServerEditDialog : public NightModeDialog
{
  QGridLayout* mainLayout;
  QHBoxLayout* buttonLayout;
  QLabel* nameLabel;
  QLabel* addressLabel;
  QLineEdit* nameEdit;
  QLineEdit* addressEdit;
  QPushButton* okButton;
  QPushButton* cancelButton;
public:
  ServerEditDialog();
  QString getName() const;
  void setName(const QString& newName);
  QString getAddr() const;
  void setAddr(const QString& newAddr);
private slots:
  void okPressed(bool);
};

#endif // SERVEREDITDIALOG_H
