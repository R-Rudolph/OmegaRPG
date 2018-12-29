#ifndef DICEROLLNOTESDIALOG_H
#define DICEROLLNOTESDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextBrowser>

#include "dialogs/nightmodedialog.h"

class DiceRollNotesDialog : public NightModeDialog
{
  Q_OBJECT
  QVBoxLayout* layout;
  QTextBrowser* notesBrowser;
  QPushButton* okButton;
public:
  explicit DiceRollNotesDialog(QWidget *parent = 0, const QString& text=QString());

signals:

public slots:
};

#endif // DICEROLLNOTESDIALOG_H
