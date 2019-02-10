#include "aliassheetreadwidget.h"
#include "objects/nightmode.h"

void AliasSheetReadWidget::updateNightMode()
{
  if(binder)
    binder->setNightMode(nightMode);
}

AliasSheetReadWidget::AliasSheetReadWidget(QWidget *parent) : QWidget(parent)
{
  nightMode = false;
  ignoreChange = false;
  updateTimer = new QTimer(this);
  updateTimer->setInterval(200);
  updateTimer->setSingleShot(true);
  new QStackedLayout(this);
  binder = nullptr;
  currentAlias = nullptr;
  connect(AliasLib::get(),&AliasLib::currentAliasChanged,this,&AliasSheetReadWidget::currentChanged);
  connect(updateTimer,&QTimer::timeout,this,&AliasSheetReadWidget::updateAlias);
  connect(this,&AliasSheetReadWidget::editedAlias,AliasLib::get(),&AliasLib::notifyChange);
  connect(NightModeController::get(),&NightModeController::nightModeChanged,this,&AliasSheetReadWidget::setNightMode);
  currentChanged(AliasLib::get()->getCurrentAlias());
}

void AliasSheetReadWidget::setNightMode(bool value)
{
  nightMode = value;
  updateNightMode();
}

void AliasSheetReadWidget::currentChanged(const Alias *alias)
{
  if(ignoreChange)
    return;
  if(alias!=nullptr)
  {
    this->currentAlias = alias;
    if(binder!=nullptr)
      delete binder;
    binder = new AliasSheetBinderWidget(alias->getBinder(),false,nullptr);
    layout()->addWidget(binder);
    updateNightMode();
    connect(binder,&AliasSheetBinderWidget::changed,this,&AliasSheetReadWidget::changed);
    connect(binder,&AliasSheetBinderWidget::chat,this,&AliasSheetReadWidget::chat);
    connect(binder,&AliasSheetBinderWidget::error,this,&AliasSheetReadWidget::error);
  }
}

void AliasSheetReadWidget::changed()
{
  updateTimer->start();
}

void AliasSheetReadWidget::updateAlias()
{
  if(currentAlias!=nullptr)
  {
    AliasLib::get()->getCurrentAliasEditable()->setBinder((AliasSheetBinder*) binder->toAliasSheet());
    ignoreChange = true;
    emit editedAlias(AliasLib::get()->getCurrentAliasEditable());
    ignoreChange = false;
  }
}
