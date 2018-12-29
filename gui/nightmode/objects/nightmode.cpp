#include "nightmode.h"

NightModeController* NightModeController::globalNMC = nullptr;

bool NightModeController::getNightMode() const
{
  return nightMode;
}

void NightModeController::setNightMode(bool value)
{
  if(value)
  {
    /*
    qApp->setStyleSheet("QWidget"
                        "{"
                        "background-color:#606070;"
                        "color:#FFFFFF;"
                        "}"
                        "QTextEdit"
                        "{"
                        "background-color:#FFFFFF;"
                        "color:#000000;"
                        "}"
                        "QMenu"
                        "{"
                        "background-color:#606070;"
                        "color:#FFFFFF;"
                        "border: 1px solid black;"
                        "}"
                        "QMenu::item::selected"
                        "{"
                        "background-color:#303040;"
                        "color:#FFFFFF;"
                        "}");
    */
    qApp->setStyleSheet("QMenuBar"
                        "{"
                        "background-color:#606070;"
                        "color:#FFFFFF;"
                        "}"
                        "QMenu"
                        "{"
                        "background-color:#606070;"
                        "color:#FFFFFF;"
                        "border: 1px solid black;"
                        "}"
                        "QMenu::item::selected"
                        "{"
                        "background-color:#303040;"
                        "color:#FFFFFF;"
                        "}");
  }
  else
  {
    qApp->setStyleSheet("");
  }
  nightMode = value;
  emit nightModeChanged(value);
}

void NightModeController::nightModeOn()
{
  setNightMode(true);
}

void NightModeController::nightModeOff()
{
  setNightMode(false);
}

void NightModeController::toggle()
{
  setNightMode(!nightMode);
}

QColor NightModeController::getOverlayColor() const
{
  if(nightMode)
    return overlayColor;
  else
    return QColor(0,0,0,0);
}

NightModeController *NightModeController::get()
{
  if(NightModeController::globalNMC==nullptr)
    NightModeController::globalNMC = new NightModeController(QColor(0,0,16,128),0);
  return NightModeController::globalNMC;
}

void NightModeController::setOverlayColor(const QColor &value)
{
  overlayColor = value;
  emit nightModeChanged(nightMode);
}

void NightModeController::setNightModeWorkaround(bool value)
{
  if(nightModeWorkaround!=value)
  {
    nightModeWorkaround = value;
    emit nightModeChanged(nightMode);
  }
}

NightModeController::NightModeController(QColor overlayColor, QObject *parent) : NightModeController(parent)
{
  this->overlayColor = overlayColor;
}

NightModeController::NightModeController(QObject *parent) : QObject(parent)
{
  nightModeWorkaround = true;;
  globalNMC = nullptr;
  nightMode = false;
}
