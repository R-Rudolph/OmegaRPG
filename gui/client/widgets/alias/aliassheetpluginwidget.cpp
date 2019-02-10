#include "aliassheetpluginwidget.h"
#include<QLabel>
#include<QQuickItem>
#include "gui/client/plugins/pluginlib.h"

AliasSheetPluginWidget::AliasSheetPluginWidget(const AliasSheetPlugin* sheet, bool edit, QWidget* parent)
  : AliasSheetWidget(edit,sheet->getSheetName(),parent)
{
  sheet_ = static_cast<AliasSheetPlugin*>(sheet->copy());
  layout_ = new QStackedLayout(this);
  display_ = PluginLib::get()->getPluginWidget(sheet->pluginDir(),this);
  if(display_ != nullptr)
  {
    pluginLoadingSuccess_ = true;
    Plugin* pluginWidget = qobject_cast<Plugin*>(display_->rootObject());
    if(pluginWidget)
    {
      pluginWidget->setPluginData(sheet->pluginData());
      pluginWidget->setEditable(edit);
      data_ = sheet_->pluginData();
      connect(pluginWidget,&Plugin::pluginDataChanged,this,&AliasSheetPluginWidget::pluginDataChanged);
      connect(pluginWidget,&Plugin::chat,this,&AliasSheetWidget::chat);
      connect(pluginWidget,&Plugin::error,this,&AliasSheetWidget::error);
    }
  }
  else
  {
    pluginLoadingSuccess_ = false;
    display_ = new QQuickWidget(this);
    display_->setSource(QUrl("qrc:/qml/ErrorMessage.qml"));
    QQuickItem* item = display_->rootObject();
    if(item!=nullptr)
      item->setProperty("errorMessage",QString("Plugin not found: %1").arg(sheet->pluginDir()));
  }
  display_->setResizeMode(QQuickWidget::SizeRootObjectToView);
  layout_->addWidget(display_);
}

AliasSheet*AliasSheetPluginWidget::toAliasSheet()
{
  if(pluginLoadingSuccess_)
  {
    AliasSheetPlugin* sheet = static_cast<AliasSheetPlugin*>(sheet_->copy());
    sheet->setPluginData(data_);
    return sheet;
  }
  else
  {
    return sheet_->copy();
  }
}

void AliasSheetPluginWidget::pluginDataChanged(const QJsonObject& data)
{
  data_ = data;
  emit changed();
}
