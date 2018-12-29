#include "nightmodetextedit.h"
#include "objects/nightmode.h"
#include "core/client/settings.h"

NightModeTextEdit::NightModeTextEdit(QWidget *parent) : QTextEdit(parent)
{
  connect(NightModeController::get(),&NightModeController::nightModeChanged,this,&NightModeTextEdit::setNightMode);
}

NightModeTextEdit::NightModeTextEdit(const QString &text, QWidget *parent) : QTextEdit(text, parent)
{
  connect(NightModeController::get(),&NightModeController::nightModeChanged,this,&NightModeTextEdit::setNightMode);
}

void NightModeTextEdit::setNightMode(bool value)
{
  if(value && orpg::Settings::get()->nightModeTextEdit)
    viewport()->setCursor(Qt::ArrowCursor);
  else
    viewport()->setCursor(Qt::IBeamCursor);
}
