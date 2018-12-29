#ifndef PLUGINLIB_H
#define PLUGINLIB_H

#include<QObject>
#include<QQmlEngine>
#include<QQmlComponent>
#include "plugin.h"
#include<QMap>
#include<QQuickWidget>
#include "plugininfo.h"

class PluginLib : public QObject
{
  Q_OBJECT
  static int typeRegisterReturn_;
  static PluginLib* lib_;

  QMap<QString,PluginInfo> plugins_;
  QQmlEngine* engine_;
public:
  explicit PluginLib(QObject *parent = nullptr);
  ~PluginLib();
  static PluginLib* get();
  PluginInfo pluginInfoByName(const QString& name);
  PluginInfo pluginInfoByDirectory(const QString& dir);
  QQuickWidget* getPluginWidget(const QString& pluginDir, QWidget* parent=nullptr);
  QMap<QString, PluginInfo> plugins() const;

signals:
  void reloaded();
public slots:
  void reload();
};

#endif // PLUGINLIB_H
