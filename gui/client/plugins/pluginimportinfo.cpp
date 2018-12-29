#include "pluginimportinfo.h"

QString PluginImportInfo::directoryName() const
{
  return directoryName_;
}

void PluginImportInfo::setDirectoryName(const QString& directoryName)
{
  directoryName_ = directoryName;
  if(directoryName_.endsWith('/'))
    directoryName_.chop(1);
}

size_t PluginImportInfo::sizeUncompressed() const
{
  return sizeUncompressed_;
}

void PluginImportInfo::setSizeUncompressed(const size_t& sizeUncompressed)
{
  sizeUncompressed_ = sizeUncompressed;
}

QString PluginImportInfo::getHumanReadableSize() const
{
  if(sizeUncompressed()<100)
    return QString("%1 B").arg(sizeUncompressed());
  else if(sizeUncompressed()<100000)
    return QString("%1 kB").arg(sizeUncompressed()/1000.0,0,'f',1);
  else if(sizeUncompressed()<100000000)
    return QString("%1 MB").arg(sizeUncompressed()/1000000.0,0,'f',1);
  else
    return QString("%1 GB").arg(sizeUncompressed()/1000000000.0,0,'f',1);
}

PluginImportInfo::PluginImportInfo(const QString& directoryName, size_t sizeUncompressed)
{
  setDirectoryName(directoryName);
  setSizeUncompressed(sizeUncompressed);
}

PluginImportInfo::PluginImportInfo()
{
  sizeUncompressed_ = 0;
}
