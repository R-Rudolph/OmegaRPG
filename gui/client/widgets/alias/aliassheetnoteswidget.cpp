#include "aliassheetnoteswidget.h"
#include "core/client/alias/aliassheetnotes.h"

AliasSheetNotesWidget::AliasSheetNotesWidget(bool edit, const QString &sheetName, QWidget *parent) : AliasSheetWidget(edit,sheetName,parent)
{
  new QStackedLayout(this);
  textEdit = new QTextEdit(this);
  textEdit->setAcceptRichText(false);
  this->layout()->addWidget(textEdit);

  connect(textEdit,&QTextEdit::textChanged,this,&AliasSheetNotesWidget::changed);
}

AliasSheetNotesWidget::AliasSheetNotesWidget(const AliasSheetNotes *sheet, bool edit, QWidget *parent)
  :AliasSheetNotesWidget(edit,sheet->getSheetName(),parent)
{
  setText(sheet->getText());
}

AliasSheet *AliasSheetNotesWidget::toAliasSheet()
{
  AliasSheet* sheet = new AliasSheetNotes(textEdit->toPlainText());
  sheet->setSheetName(sheetName);
  return sheet;
}

void AliasSheetNotesWidget::setText(const QString &text)
{
  textEdit->setPlainText(text);
  emit changed();
}
