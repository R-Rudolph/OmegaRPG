#include "version.h"

namespace orpg
{
  const QString Version::seperator = ".";

  Version::Version(const QList<uint>& versions)
  {
    this->_versions = versions;
  }

  Version::Version(const QString& version)
  {
    QStringList versions = version.split(seperator);
    QList<uint> versionInteger;
    bool ok;
    foreach(const QString& string, versions)
    {
      versionInteger.append(string.toUInt(&ok));
      if(!ok)
          break;
    }
    if(ok)
      this->_versions = versionInteger;
  }

  bool Version::operator <(const Version& other) const
  {
    int minsize = qMin(_versions.size(),other._versions.size());
    for(int i=0;i<minsize;i++)
    {
      if(_versions[i]!=other._versions[i])
        return _versions[i] < other._versions[i];
    }
    if(_versions.size()!=other._versions.size())
    {
      return _versions.size()<other._versions.size();
    }
    return false;
  }

  bool Version::isValid() const
  {
    return !_versions.isEmpty();
  }

  QString Version::toString() const
  {
    QStringList list;
    foreach(uint value, _versions)
      list.append(QString::number(value));
    return list.join(seperator);
  }


}
