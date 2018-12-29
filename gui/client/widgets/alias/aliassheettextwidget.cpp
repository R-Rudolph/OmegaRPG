#include "aliassheettextwidget.h"
#include "core/client/alias/aliassheettext.h"

AliasSheetTextWidget::AliasSheetTextWidget(bool edit, const QString& sheetName, QWidget *parent) : AliasSheetWidget(edit,sheetName,parent)
{
  new QStackedLayout(this);
  if(isEditWidget)
    textEdit = new TextEditHtmlInsert(this);
  else
  {
    QTextBrowser* browser = new QTextBrowser(this);
    browser->setOpenExternalLinks(true);
    textEdit = browser;
  }
  this->layout()->addWidget(textEdit);
  connect(textEdit,&QTextEdit::textChanged,this,&AliasSheetTextWidget::textChangedSlot);
}

AliasSheetTextWidget::AliasSheetTextWidget(const AliasSheetText *sheet, bool edit, QWidget *parent)
  :AliasSheetTextWidget(edit,sheet->getSheetName(),parent)
{
  setText(sheet->getText());
}

AliasSheet *AliasSheetTextWidget::toAliasSheet()
{
  if(isEditWidget)
    return new AliasSheetText(sheetName,text);
  else
    return new AliasSheetText(sheetName,text);
}

void AliasSheetTextWidget::setText(const QString &text)
{
  this->text = text;
  if(isEditWidget)
    textEdit->setPlainText(text);
  else
    textEdit->setText(text);
}

void AliasSheetTextWidget::textChangedSlot()
{
  if(isEditWidget)
  {
    this->text = textEdit->toPlainText();
    emit changed();
  }
}
