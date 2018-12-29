#ifndef INITIATIVEDIALOG_H
#define INITIATIVEDIALOG_H

#include <QObject>
#include <QDialog>

#include "dialogs/nightmodedialog.h"

class ColorSelectButton;
class QVBoxLayout;
class QHBoxLayout;
class QFormLayout;
class QPushButton;
class QLineEdit;
class QSpinBox;
class InitiativeResource;
class ImageUrlSelection;

class InitiativeDialog : public NightModeDialog
{
  Q_OBJECT
  QVBoxLayout* mainLayout;
  QFormLayout* formLayout;
  QLineEdit* nameEdit;
  ColorSelectButton* colorButton;
  ImageUrlSelection* iconUrlEdit;
  QHBoxLayout* initiativeLayout;
  QSpinBox* initiativeBox;
  QPushButton* initiativeRollButton;
  QLineEdit* statusEdit;
  QHBoxLayout* buttonLayout;
  QPushButton* okButton;
  QPushButton* cancelButton;
public:
  explicit InitiativeDialog(QWidget *parent = nullptr);
  void setresource(const InitiativeResource& resource);
  InitiativeResource getresource();
  void clear();
signals:
public slots:
  virtual int exec();
private slots:
  void addD20();
};

#endif // INITIATIVEDIALOG_H
