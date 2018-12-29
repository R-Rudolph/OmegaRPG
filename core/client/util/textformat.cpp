#include "textformat.h"

bool TextFormat::getBold() const
{
  return bold;
}

void TextFormat::setBold(bool value)
{
  bold = value;
}

bool TextFormat::getItalic() const
{
  return italic;
}

void TextFormat::setItalic(bool value)
{
  italic = value;
}

bool TextFormat::getUnderline() const
{
  return underline;
}

void TextFormat::setUnderline(bool value)
{
  underline = value;
}

QColor TextFormat::getTextColor() const
{
  return textColor;
}

void TextFormat::setTextColor(const QColor &value)
{
  textColor = value;
}

TextFormat::TextFormat()
{
  bold = false;
  italic = false;
  underline = false;
  textColor = Qt::black;
}

TextFormat::TextFormat(const QTextCharFormat &format)
{
  bold = format.font().bold();
  italic = format.font().italic();
  underline = format.font().underline();
  textColor = format.foreground().color();
  if(format.foreground()==Qt::NoBrush)
    textColor = QColor::Invalid;
}

TextFormat::TextFormat(const QTextCursor &cursor) : TextFormat(cursor.charFormat())
{

}

bool TextFormat::operator==(const TextFormat &other)
{
  return (this->bold == other.bold) &&
      (this->italic == other.italic) &&
      (this->underline == other.underline) &&
      (this->textColor == other.textColor);
}

bool TextFormat::operator!=(const TextFormat &other)
{
  return !(this->operator ==(other));
}

QString TextFormat::wrapText(const QString &text, const QColor& neutralColor, const QColor& replacementColor, bool replace)
{
  QString style;
  if(bold)
    style.append("font-weight: bold;");
  if(italic)
    style.append("font-style: italic;");
  if(underline)
    style.append("text-decoration: underline;");
  if(textColor!=neutralColor && textColor!=QColor::Invalid)
    style.append("color:"+textColor.name()+";");
  else if(replace)
    style.append("color:"+replacementColor.name()+";");
  if(style.isEmpty())
    return text;
  else
  {
    return "<span style=\""+style+"\">"+text+"</span>";
  }
}

QString TextFormat::getHtml(QTextEdit *textEdit, const QColor& neutralColor)
{
  QString result;
  QTextCursor cursor = textEdit->textCursor();
  for(QTextBlock block = textEdit->document()->begin();block!=textEdit->document()->end();block = block.next())
  {
    cursor.setPosition(block.position(),QTextCursor::MoveAnchor);
    TextFormat currentFormat(cursor);
    for(int i=block.position();block.contains(i);i++)
    {
      cursor.setPosition(i,QTextCursor::KeepAnchor);
      TextFormat format(cursor);
      if(currentFormat!=format)
      {
        i--;
        cursor.setPosition(i,QTextCursor::KeepAnchor);
        result.append(currentFormat.wrapText(cursor.selectedText().toHtmlEscaped(),neutralColor));
        currentFormat = format;
        cursor.setPosition(i,QTextCursor::MoveAnchor);
      }
    }
    result.append(currentFormat.wrapText(cursor.selectedText().toHtmlEscaped(),neutralColor));
    if(block.next()!=textEdit->document()->end())
      result.append("\n");
  }
  return result;
}
