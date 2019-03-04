#ifndef CHANNELNAME_H
#define CHANNELNAME_H

#include<QMap>

namespace orpg
{
  class ChannelName
  {
    QString first_, second_;
    enum ChannelType
    {
      ChannelAll,
      ChannelGM,
      ChannelMixed,
      ChannelPrivate
    };
    ChannelType type_;
    QString toNameWithId(const QString& id, const QMap<QString,QString>& mapping) const;
  public:
    ChannelName();
    ChannelName(const QString& from, const QString& to="");
    QString rawName() const;
    QString name(const QMap<QString,QString>& mapping) const;

    bool isAll() const;
    bool isGM() const;
    bool isMixed() const;
    bool isPrivate() const;

    bool operator<(const ChannelName& other) const;
  };
}


#endif // CHANNELNAME_H
