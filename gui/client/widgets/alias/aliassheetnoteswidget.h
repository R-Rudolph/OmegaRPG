#ifndef ALIASSHEETNOTESWIDGET_H
#define ALIASSHEETNOTESWIDGET_H

#include <QWidget>
#include <QStackedLayout>
#include <QTextEdit>
#include "aliassheetwidget.h"

class AliasSheetNotes;

class AliasSheetNotesWidget : public AliasSheetWidget
{
  Q_OBJECT
  QTextEdit* textEdit;
public:
  explicit AliasSheetNotesWidget(bool edit, const QString& sheetName, QWidget *parent = 0);
  explicit AliasSheetNotesWidget(const AliasSheetNotes* sheet, bool edit, QWidget *parent = 0);
  virtual AliasSheet* toAliasSheet();
signals:
private slots:
public slots:
  void setText(const QString& text);
};

#endif // ALIASSHEETNOTESWIDGET_H
