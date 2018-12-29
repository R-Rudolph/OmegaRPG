#ifndef VERSION_H
#define VERSION_H

#include<QString>
#include<QList>

namespace orpg
{
  class Version
  {
    static const QString seperator;
    QList<uint32_t> _versions;
  public:
    Version(const QList<uint>& versions);
    Version(const QString& version);
    bool operator <(const Version& other) const;
    bool isValid() const;
    QString toString() const;
  };
}

#endif // VERSION_H
