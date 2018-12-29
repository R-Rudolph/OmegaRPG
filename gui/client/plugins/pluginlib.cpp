#include "pluginlib.h"
#include "core/multi/util/global.h"
#include<QQuickItem>
#include<QDir>
#include<QString>

int PluginLib::typeRegisterReturn_ = qmlRegisterType<Plugin>("Plugin", 1,0, "Plugin");
PluginLib* PluginLib::lib_ = nullptr;

QMap<QString, PluginInfo> PluginLib::plugins() const
{
  return plugins_;
}

PluginLib::PluginLib(QObject *parent) : QObject(parent)
{
  engine_ = nullptr;
  reload();
}

PluginLib::~PluginLib()
{

}

PluginLib*PluginLib::get()
{
  if(lib_==nullptr)
    lib_ = new PluginLib();
  return lib_;
}

void PluginLib::reload()
{
  plugins_.clear();
  delete engine_;
  engine_ = new QQmlEngine(this);
  QDir pluginDir(Global::pluginsFolder());
  foreach(const QString& subdirname, pluginDir.entryList(QDir::Dirs|QDir::NoDotAndDotDot))
  {
    PluginInfo plugin(pluginDir.absoluteFilePath(subdirname),engine_);
    plugins_.insert(subdirname,plugin);
  }
  emit reloaded();
}

PluginInfo PluginLib::pluginInfoByName(const QString& name)
{
  foreach(const PluginInfo& plugin, plugins_)
  {
    if(plugin.name() == name)
      return plugin;
  }
  return PluginInfo();
}

PluginInfo PluginLib::pluginInfoByDirectory(const QString& dir)
{
  return plugins_[dir];
}

QQuickWidget* PluginLib::getPluginWidget(const QString& pluginDir, QWidget* parent)
{
  PluginInfo pluginInfo = plugins_[pluginDir];
  if(pluginInfo.isValid())
  {
    return pluginInfo.createWidget(parent);
  }
  else
    return nullptr;
}
