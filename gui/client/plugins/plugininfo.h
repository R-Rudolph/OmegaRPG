#ifndef PLUGININFO_H
#define PLUGININFO_H

#include<QString>
#include<QQmlEngine>
#include<QQuickWidget>
#include<QObject>

class PluginInfo
{
  QString name_;
  QString description_;
  QString qmlFile_;
  QList<QString> errors_;
  bool valid_;
public:
  PluginInfo();
  PluginInfo(QString directory, QQmlEngine* engine);
  QString name() const;
  QString description() const;
  QString qmlFile() const;
  QList<QString> errors() const;
  bool isValid() const;
  QQuickWidget* createWidget(QWidget* parent=nullptr) const;
};

#endif // PLUGININFO_H
