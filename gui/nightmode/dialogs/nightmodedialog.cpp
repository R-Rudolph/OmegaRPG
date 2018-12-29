#include "nightmodedialog.h"

NightModeDialog::NightModeDialog(QWidget *parent) : QDialog(parent)
{
  setLayout(new QStackedLayout());
  overlay = new NightModeOverlay(this);
  overlay->setWidget(new QWidget(overlay));
  layout()->addWidget(overlay);
}

QWidget *NightModeDialog::getCentralWidget()
{
  return overlay->getWidget();
}
