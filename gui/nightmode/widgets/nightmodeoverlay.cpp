#include "nightmodeoverlay.h"
#include "objects/nightmode.h"

NightModeOverlay::NightModeOverlay(QWidget *parent, Qt::WindowFlags f) : ColorOverlayWrapperWidget(parent,f)
{
  connect(NightModeController::get(),&NightModeController::nightModeChanged,this,&NightModeOverlay::updateOverlay);
  setColor(NightModeController::get()->getOverlayColor());
}

void NightModeOverlay::updateOverlay(bool)
{
  setColor(NightModeController::get()->getOverlayColor());
}
