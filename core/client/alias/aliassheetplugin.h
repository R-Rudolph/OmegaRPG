#ifndef ALIASSHEETPLUGIN_H
#define ALIASSHEETPLUGIN_H

#include "aliassheet.h"

class AliasSheetPlugin : public AliasSheet
{
  QString pluginDir_;
  QJsonObject pluginData_;
public:
  const static QString className;
  AliasSheetPlugin();
  AliasSheetPlugin(const QString& sheetName, const QString& pluginDir);
  AliasSheetPlugin(const QJsonObject& sheetObject);
  virtual QJsonObject toJson() const;
  virtual void loadJson(const QJsonObject& sheetObject);
  virtual AliasSheet* copy() const;
  virtual SheetType getType() const;
  QString pluginDir() const;
  QJsonObject pluginData() const;
  QString pluginDataString() const;
  void setPluginData(const QJsonObject& pluginData);
};

#endif // ALIASSHEETPLUGIN_H
