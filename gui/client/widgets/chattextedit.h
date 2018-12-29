#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QWidget>
#include <QTextEdit>
#include <QKeyEvent>
#include <QString>
#include <QVector>
#include <QPlainTextEdit>
#include <QTextEdit>

#include <QTextDocument>
#include <QTextBlock>

#include "core/client/util/textformat.h"
#include "core/client/settings.h"
#include "widgets/nightmodetextedit.h"



class ChatTextEdit : public NightModeTextEdit
{
  Q_OBJECT
  QColor neutralColor;
  QColor currentColor;
  bool colorChangedByTextEdit;
  QVector<QString> history;
  int historyIt;
  bool resettingText;
  void prevHistory();
  void nextHistory();
  bool isBold();
  bool isItalic();
  bool isUnderline();
  void resetText();
  void publishFormat();
public:
  bool sendKeyPressed;
  explicit ChatTextEdit(QWidget *parent = 0);

  void setTextColorPalette(const QColor& color);
signals:
  void typedMessage(QString);
  void macroKey(int);
public slots:
  void keyPressEvent( QKeyEvent * event );
  void toggleBold();
  void toggleItalic();
  void toggleUnderline();
  void overwriteTextColor(const QColor& color);
  void setTextColor(const QColor &c);
private slots:
  void textChangedSlot();
};

#endif // MYTEXTEDIT_H
