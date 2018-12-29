#include "textedithtmlinsert.h"

TextEditHtmlInsert::TextEditHtmlInsert(QWidget *parent) : QTextEdit(parent)
{

}

void TextEditHtmlInsert::insertFromMimeData(const QMimeData *source)
{
  if(source->hasHtml())
  {
    QString htmlString = source->html();
    QString str = "<!--StartFragment-->";
    int index = htmlString.indexOf(str);
    if(index>=0)
    {
      htmlString.remove(0,index+str.size());
    }
    str = "<!--EndFragment-->";
    index = htmlString.lastIndexOf(str);
    if(index>=0)
    {
      htmlString.chop(htmlString.size()-index);
    }
    insertPlainText(htmlString);
  }
  else if(source->hasText())
  {
    insertPlainText(source->text());
  }
}

QMimeData *TextEditHtmlInsert::createMimeDataFromSelection() const
{
  QMimeData* result = QTextEdit::createMimeDataFromSelection();
  QString text = result->text();
  result->clear();
  result->setText(text);
  return result;
}
