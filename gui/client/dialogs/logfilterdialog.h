#ifndef LOGFILTERDIALOG2_H
#define LOGFILTERDIALOG2_H

#include <QWidget>
#include <QDialog>
#include <QRegularExpression>

#include "core/client/log/logfilter.h"
#include "dialogs/nightmodedialog.h"

class QPushButton;
class QComboBox;
class QLineEdit;
class QHBoxLayout;
class QCheckBox;

class LogFilterDialog : public NightModeDialog
{
  Q_OBJECT
  QPushButton* okButton;
  QPushButton* cancelButton;
  QComboBox* fieldBox;
  QComboBox* matchBox;
  QLineEdit* stringEdit;
  QComboBox* invertFilterBox;
  QHBoxLayout* buttonLayout;

  QRegularExpression regex;
public:
  explicit LogFilterDialog(QWidget *parent = 0);
  orpg::LogFilter getFilter();
signals:

private slots:
  void okClicked();
  void changed();
};

#endif // LOGFILTERDIALOG2_H
