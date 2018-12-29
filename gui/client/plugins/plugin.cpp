#include "plugin.h"

Plugin::Plugin(QQuickItem *parent) : QQuickItem(parent)
{

}

QString Plugin::pluginName() const
{
  return pluginName_;
}

void Plugin::setPluginName(const QString& pluginName)
{
  pluginName_ = pluginName;
}

QString Plugin::description() const
{
  return description_;
}

void Plugin::setDescription(const QString& description)
{
  description_ = description;
}

QJsonObject Plugin::pluginData() const
{
  return pluginData_;
}

void Plugin::setPluginData(const QJsonObject& data)
{
  pluginData_ = data;
  emit pluginDataSet();
}

bool Plugin::editable() const
{
  return editable_;
}

void Plugin::setEditable(bool editable)
{
  editable_ = editable;
  emit editableChanged();
}

