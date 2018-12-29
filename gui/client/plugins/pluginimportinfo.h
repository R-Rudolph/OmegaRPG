#ifndef PLUGINIMPORTINFO_H
#define PLUGINIMPORTINFO_H

#include<QString>

class PluginImportInfo
{
  QString directoryName_;
  size_t sizeUncompressed_;
public:
  PluginImportInfo(const QString& directoryName, size_t sizeUncompressed);
  PluginImportInfo();
  QString directoryName() const;
  void setDirectoryName(const QString& directoryName);
  size_t sizeUncompressed() const;
  void setSizeUncompressed(const size_t& sizeUncompressed);
  QString getHumanReadableSize() const;
};

#endif // PLUGINIMPORTINFO_H
