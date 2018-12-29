#ifndef FILEHASHURLTABLE_H
#define FILEHASHURLTABLE_H

#include<QMap>
#include<QByteArray>
#include<QString>
#include<QCryptographicHash>
#include<QJsonArray>

class FileHashUrlTable
{
  FileHashUrlTable(const QString& filepath);
  QMap<QByteArray,QString> m_hashUrlMap;
  QString m_filepath;

  QJsonArray mapToJson() const;
  void loadMapFromJson(const QJsonArray array);
  bool saveMap(const QString& filepath) const;
  bool loadMap(const QString& filepath);
public:
  static FileHashUrlTable& get();
  static QByteArray hash(QIODevice* device);

  void insert(const QByteArray& hash, const QString& url);
  bool contains(const QByteArray& hash) const;
  const QString operator[](const QByteArray& hash) const;

  FileHashUrlTable(FileHashUrlTable const&) = delete;
  void operator=(FileHashUrlTable const&)   = delete;
};

#endif // FILEHASHURLTABLE_H
