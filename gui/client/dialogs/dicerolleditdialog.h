#ifndef DICEROLLEDITDIALOG_H
#define DICEROLLEDITDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QCheckBox>

#include "core/client/alias/aliasdiceroll.h"
#include "dialogs/nightmodedialog.h"

class DiceBarWidget;

class DiceRollEditDialog : public NightModeDialog
{
  Q_OBJECT
  QFormLayout* mainLayout;
  QHBoxLayout* buttonLayout;
  QLineEdit* nameEdit;
  DiceBarWidget* diceBar;
  QLineEdit* bonusEdit;
  QCheckBox* rollParentBox;
  QTextEdit* notesEdit;
  QPushButton* okButton;
  QPushButton* cancelButton;
public:
  explicit DiceRollEditDialog(QWidget *parent = 0);
  void setName(const QString& name);
  void setBonus(const QString& bonus);
  void setNotes(const QString& notes);
  void setRollParent(bool value);
  QString getName() const;
  QString getBonus() const;
  QString getNotes() const;
  bool getRollParent() const;
signals:

private slots:
  void addDiceRoll(int num, int value);
  void bonusChanged();
};

#endif // DICEROLLEDITDIALOG_H
