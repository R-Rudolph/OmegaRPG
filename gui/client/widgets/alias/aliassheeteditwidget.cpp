#include "aliassheeteditwidget.h"
#include "objects/nightmode.h"

void AliasSheetEditWidget::updateNightMode()
{
  if(binder)
    binder->setNightMode(nightMode);
}

AliasSheetEditWidget::AliasSheetEditWidget(QWidget *parent) : QWidget(parent)
{
  nightMode = false;
  new QStackedLayout(this);
  binder = nullptr;
  currentAlias = nullptr;
  connect(NightModeController::get(),&NightModeController::nightModeChanged,this,&AliasSheetEditWidget::setNightMode);
}

void AliasSheetEditWidget::setAlias(Alias *alias)
{
  if(alias!=nullptr)
  {
    this->currentAlias = alias;
    if(binder!=nullptr)
      delete binder;
    binder = new AliasSheetBinderWidget(alias->getBinder(),true,nullptr);
    layout()->addWidget(binder);
    updateNightMode();
    connect(binder,&AliasSheetBinderWidget::changed,this,&AliasSheetEditWidget::changed);
  }
}

void AliasSheetEditWidget::setNightMode(bool value)
{
  nightMode = value;
  updateNightMode();
}

void AliasSheetEditWidget::changed()
{
  if(currentAlias!=nullptr)
  {
    currentAlias->setBinder((AliasSheetBinder*) binder->toAliasSheet());
    emit editedAlias(currentAlias);
    emit somethingChanged();
  }
}
