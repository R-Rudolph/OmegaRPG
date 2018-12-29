#ifndef ALIASSHEETTEXTWIDGET_H
#define ALIASSHEETTEXTWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QTextBrowser>
#include "widgets/textedithtmlinsert.h"
#include "aliassheetwidget.h"

class AliasSheetText;

class AliasSheetTextWidget : public AliasSheetWidget
{
  Q_OBJECT
  QString text;
  QTextEdit* textEdit;
public:
  explicit AliasSheetTextWidget(bool edit, const QString& sheetName, QWidget *parent = 0);
  explicit AliasSheetTextWidget(const AliasSheetText* sheet, bool edit, QWidget *parent = 0);
  virtual AliasSheet* toAliasSheet();
  void setText(const QString& text);
signals:
public slots:
private slots:
  void textChangedSlot();
};

#endif // ALIASSHEETTEXTWIDGET_H
