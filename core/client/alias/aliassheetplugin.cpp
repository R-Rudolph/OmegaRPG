#include "aliassheetplugin.h"
#include<QJsonDocument>

const QString AliasSheetPlugin::className = "plugin";

QString AliasSheetPlugin::pluginDir() const
{
  return pluginDir_;
}

QJsonObject AliasSheetPlugin::pluginData() const
{
  return pluginData_;
}

QString AliasSheetPlugin::pluginDataString() const
{
  return QJsonDocument(pluginData_).toJson(QJsonDocument::Compact);
}

void AliasSheetPlugin::setPluginData(const QJsonObject& pluginData)
{
  pluginData_ = pluginData;
}

AliasSheetPlugin::AliasSheetPlugin()
{

}

AliasSheetPlugin::AliasSheetPlugin(const QString& sheetName, const QString& pluginName)
{
  this->sheetName = sheetName;
  pluginDir_ = pluginName;
}

AliasSheetPlugin::AliasSheetPlugin(const QJsonObject& sheetObject)
{
  loadJson(sheetObject);
}

QJsonObject AliasSheetPlugin::toJson() const
{
  QJsonObject result;
  result.insert("type",AliasSheetPlugin::className);
  result.insert("name",sheetName);
  result.insert("plugin",pluginDir_);
  result.insert("data",pluginData_);
  return result;
}

void AliasSheetPlugin::loadJson(const QJsonObject& sheetObject)
{
  pluginData_ = sheetObject["data"].toObject();
  pluginDir_ = sheetObject["plugin"].toString();
  sheetName = sheetObject["name"].toString();
}

AliasSheet* AliasSheetPlugin::copy() const
{
  AliasSheet* sheet = new AliasSheetPlugin(*this);
  return sheet;
}

AliasSheet::SheetType AliasSheetPlugin::getType() const
{
  return SHEET_PLUGIN;
}
