#ifndef TEXTFORMAT_H
#define TEXTFORMAT_H

#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextEdit>
#include <QTextBlock>
#include <QXmlStreamReader>


class TextFormat
{
  bool bold;
  bool italic;
  bool underline;
  QColor textColor;
public:
  TextFormat();
  TextFormat(const QTextCharFormat& format);
  TextFormat(const QTextCursor& cursor);
  bool operator==(const TextFormat& other);
  bool operator!=(const TextFormat& other);
  QString wrapText(const QString& text, const QColor& neutralColor, const QColor& replacementColor=Qt::black, bool replace=false);

  static QString getHtml(QTextEdit* textEdit, const QColor& neutralColor);
  bool getBold() const;
  void setBold(bool value);
  bool getItalic() const;
  void setItalic(bool value);
  bool getUnderline() const;
  void setUnderline(bool value);
  QColor getTextColor() const;
  void setTextColor(const QColor &value);
};

#endif // TEXTFORMAT_H
