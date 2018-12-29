#ifndef MAPTEXTDIALOG_H
#define MAPTEXTDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QSpinBox>
#include <QLineEdit>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>

#include "dialogs/nightmodedialog.h"

class MapTextDialog : public NightModeDialog
{
  Q_OBJECT
  QFormLayout* layout;
  QHBoxLayout* buttonLayout;
  QLineEdit* textEdit;
  QSpinBox* sizeBox;
  QPushButton* okButton;
  QPushButton* cancelButton;
public:
  explicit MapTextDialog(QWidget *parent = 0);
  QString getText();
  int getSize();
  void setText(const QString& text);
  void setSize(int size);
signals:

private slots:
  void okClicked();
};

#endif // MAPTEXTDIALOG_H
