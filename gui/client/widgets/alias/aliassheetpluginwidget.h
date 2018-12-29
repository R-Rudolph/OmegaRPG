#ifndef ALIASSHEETPLUGINWIDGET_H
#define ALIASSHEETPLUGINWIDGET_H

#include <QObject>
#include <QWidget>
#include <QTextBrowser>
#include <QQuickWidget>
#include "aliassheetwidget.h"
#include "core/client/alias/aliassheetplugin.h"

class AliasSheetPluginWidget : public AliasSheetWidget
{
  Q_OBJECT
  QStackedLayout* layout_;
  QQuickWidget* display_;
  //data
  AliasSheetPlugin* sheet_;
  QJsonObject data_;

  bool pluginLoadingSuccess_;
public:
  explicit AliasSheetPluginWidget(const AliasSheetPlugin* sheet, bool edit, QWidget *parent = nullptr);
  virtual AliasSheet* toAliasSheet();
signals:

private slots:
  void pluginDataChanged(const QJsonObject& data);
};

#endif // ALIASSHEETPLUGINWIDGET_H
