#include "plugininfo.h"
#include<QFile>
#include<QDir>
#include<QQmlComponent>
#include "plugin.h"

QString PluginInfo::name() const
{
  return name_;
}

QString PluginInfo::description() const
{
  return description_;
}

QString PluginInfo::qmlFile() const
{
  return qmlFile_;
}

QList<QString> PluginInfo::errors() const
{
  return errors_;
}

bool PluginInfo::isValid() const
{
  return valid_;
}

QQuickWidget*PluginInfo::createWidget(QWidget* parent) const
{
  QQuickWidget* widget = new QQuickWidget(parent);
  widget->setSource(QUrl::fromLocalFile(qmlFile_));
  QQuickItem* item = widget->rootObject();
  Plugin* plugin = qobject_cast<Plugin*>(item);
  if(plugin!=nullptr)
  {
    return widget;
  }
  else
  {
    delete widget;
    return nullptr;
  }
}

PluginInfo::PluginInfo()
{
  valid_ = false;
}

PluginInfo::PluginInfo(QString directory, QQmlEngine* engine)
{
  valid_ = false;
  QDir dir(directory);
  qmlFile_ = dir.absoluteFilePath("plugin.qml");
  if(!QFile(qmlFile_).exists())
  {
    qDebug() << directory << "1";
    errors_.push_back(QString("Plugin directory '%1' contains no plugin.qml.").arg(directory));
    return;
  }
  QQmlComponent* component = new QQmlComponent(engine,QUrl::fromLocalFile(qmlFile_));
  if(!component->errors().isEmpty())
  {
    qDebug() << directory << "2" << component->errors();
    qDebug() << "string" << component->errorString();
    errors_.push_back(component->errorString());
    component->deleteLater();
    return;
  }
  QObject* object = component->create();
  Plugin* plugin = qobject_cast<Plugin*>(object);
  if(plugin==nullptr)
  {
    qDebug() << directory << "3";
    errors_.push_back(QString("%1: Root Object is not of 'Plugin' type.").arg(qmlFile_));
    component->deleteLater();
    object->deleteLater();
    return;
  }
  name_ = plugin->pluginName();
  description_ = plugin->description();
  if(name_.isEmpty())
  {
    qDebug() << directory << "4";
    errors_.push_back(QString("%1: pluginName property not set.").arg(qmlFile_));
    component->deleteLater();
    object->deleteLater();
    return;
  }
  component->deleteLater();
  object->deleteLater();
  valid_ = true;
}
