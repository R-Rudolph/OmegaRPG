#include "chattextedit.h"
#include "objects/nightmode.h"

void ChatTextEdit::prevHistory()
{
  if(history.size()==0)
    return;
  historyIt = std::max(0,historyIt-1);
  setHtml("<span style=\"color:"+neutralColor.name()+";\">"+history[historyIt]+"</span>");
}

void ChatTextEdit::nextHistory()
{
  if(history.size()==0)
    return;
  historyIt = std::min(history.size(),historyIt+1);
  if(historyIt<history.size())
    setHtml("<span style=\"color:"+neutralColor.name()+";\">"+history[historyIt]+"</span>");
  else
  {
    setHtml(QString());
    resetText();
  }
}

bool ChatTextEdit::isBold()
{
  return this->textCursor().charFormat().font().bold();
}

bool ChatTextEdit::isItalic()
{
  return this->textCursor().charFormat().font().italic();
}

void ChatTextEdit::resetText()
{
  setPlainText(QString());
  setFontWeight(QFont::Normal);
  setFontItalic(false);
  setFontUnderline(false);
  QTextCharFormat format = currentCharFormat();
  format.clearForeground();
  setCurrentCharFormat(format);
  //setTextColor(neutralColor);
}

void ChatTextEdit::publishFormat()
{
  emit currentCharFormatChanged(currentCharFormat());
}

void ChatTextEdit::toggleBold()
{
  bool value = !isBold();
  if(value)
    setFontWeight(QFont::Bold);
  else
    setFontWeight(QFont::Normal);
  publishFormat();
}

void ChatTextEdit::toggleItalic()
{
  setFontItalic(!isItalic());
  publishFormat();
}

void ChatTextEdit::toggleUnderline()
{
  setFontUnderline(!fontUnderline());
  publishFormat();
}

void ChatTextEdit::overwriteTextColor(const QColor &color)
{
  //QString replacementText = TextFormat::getColorReplaceText(this,neutralColor,color);
  //setHtml(replacementText);
  QTextCharFormat format = currentCharFormat();
  format.clearForeground();
  setCurrentCharFormat(format);
  setStyleSheet("color:"+color.name());
  neutralColor = color;
  //setTextColor(color);
  //publishFormat();
}

void ChatTextEdit::setTextColor(const QColor &c)
{
  QTextCharFormat format = currentCharFormat();
  if(c == neutralColor)
    format.clearForeground();
  else
    format.setForeground(QBrush(c));
  QTextEdit::setCurrentCharFormat(format);
}

void ChatTextEdit::textChangedSlot()
{
  if(this->toPlainText().isEmpty())
  {
    if(resettingText)
      return;
    resettingText = true;
    resetText();
    resettingText = false;
  }
}

ChatTextEdit::ChatTextEdit(QWidget *parent) : NightModeTextEdit(parent)
{
  sendKeyPressed = true;
  colorChangedByTextEdit = false;
  historyIt = 0;
  setAcceptRichText(false);
  setTabChangesFocus(false);
  resettingText = false;
  connect(this,&ChatTextEdit::textChanged,this,&ChatTextEdit::textChangedSlot);
}

void ChatTextEdit::keyPressEvent ( QKeyEvent * event )
{
  if(event->modifiers() & Qt::ControlModifier)
  { //ctrl functions
    switch(event->key())
    {
      case Qt::Key_Return:
      case Qt::Key_Enter:
      {
        insertPlainText("\n");
        if(this->toPlainText().isEmpty())
          resetText();
        break;
      }
      case Qt::Key_B:
      {
        toggleBold();
        break;
      }
      case Qt::Key_I:
      {
        toggleItalic();
        break;
      }
      case Qt::Key_U:
      {
        toggleUnderline();
        break;
      }
      case Qt::Key_P:
      {
        sendKeyPressed = !sendKeyPressed;
        break;
      }
      case Qt::Key_Up:
      {
        prevHistory();
        break;
      }
      case Qt::Key_Down:
      {
        nextHistory();
        break;
      }
      case Qt::Key_Plus:
      {
        orpg::Settings::get()->fontSizeIncrease();
        break;
      }
      case Qt::Key_Minus:
      {
        orpg::Settings::get()->fontSizeDecrease();
        break;
      }
      default:
      {
        //if(this->toPlainText().isEmpty())
        //  setTextColor(neutralColor);
        colorChangedByTextEdit = true;
        QTextEdit::keyPressEvent( event );
        colorChangedByTextEdit = false;
        break;
      }
    }
    event->accept();
  }
  else
  { //regular keys
    switch(event->key())
    {
      case Qt::Key_Return:
      case Qt::Key_Enter:
      {
        QString string = TextFormat::getHtml(this,neutralColor);
        if(string.isEmpty())
          break;
        if(history.isEmpty() || (!history.isEmpty() && (string!=history.last())))
        {
          history.push_back(string);
          historyIt = history.size();
        }
        emit typedMessage(string);
        resetText();
        event->accept();
        break;
      }
      case Qt::Key_PageUp:
      {
        prevHistory();
        event->accept();
        break;
      }
      case Qt::Key_PageDown:
      {
        nextHistory();
        event->accept();
        break;
      }
      case Qt::Key_F1:
        emit macroKey(0);
        event->accept();
        break;
      case Qt::Key_F2:
        emit macroKey(1);
        event->accept();
        break;
      case Qt::Key_F3:
        emit macroKey(2);
        event->accept();
        break;
      case Qt::Key_F4:
        emit macroKey(3);
        event->accept();
        break;
      case Qt::Key_F5:
        emit macroKey(4);
        event->accept();
        break;
      case Qt::Key_F6:
        emit macroKey(5);
        event->accept();
        break;
      case Qt::Key_F7:
        emit macroKey(6);
        event->accept();
        break;
      case Qt::Key_F8:
        emit macroKey(7);
        event->accept();
        break;
      case Qt::Key_F9:
        emit macroKey(8);
        event->accept();
        break;
      case Qt::Key_F10:
        emit macroKey(9);
        event->accept();
        break;
      case Qt::Key_F11:
        emit macroKey(10);
        event->accept();
        break;
      case Qt::Key_F12:
        emit macroKey(11);
        event->accept();
        break;
      case Qt::Key_F13:
        emit macroKey(12);
        event->accept();
        break;
      default:
      {
        //if(this->toPlainText().isEmpty())
        //  setTextColor(neutralColor);
        colorChangedByTextEdit = true;
        QTextEdit::keyPressEvent( event );
        colorChangedByTextEdit = false;
        break;
      }
    }
  }

}
