#include "filehashurltable.h"
#include <QSaveFile>
#include <QFile>
#include <QJsonDocument>
#include <QCryptographicHash>

FileHashUrlTable::FileHashUrlTable(const QString& filepath)
{
  this->m_filepath = filepath;
  loadMap(filepath);
}

QJsonArray FileHashUrlTable::mapToJson() const
{
  QJsonArray data;
  for(auto i = m_hashUrlMap.begin();i!=m_hashUrlMap.end();++i)
  {
    QJsonArray entry;
    entry.append(QString::fromUtf8(i.key().toBase64()));
    entry.append(i.value());
    data.append(entry);
  }
  return data;
}

void FileHashUrlTable::loadMapFromJson(const QJsonArray array)
{
  m_hashUrlMap.clear();
  for(int i=0;i<array.size();i++)
  {
    QJsonArray entry = array[i].toArray();
    if(entry.size()==2)
    {
      QByteArray hash = QByteArray::fromBase64(entry[0].toString().toUtf8());
      QString url = entry[1].toString();
      if(hash.size()>0 && url.size() > 0)
        m_hashUrlMap.insert(hash,url);
    }
  }
}

bool FileHashUrlTable::saveMap(const QString &filepath) const
{
  if(filepath.isEmpty())
    return false;
  QSaveFile file(filepath);
  if(file.open(QIODevice::WriteOnly))
  {
    file.write(QJsonDocument(mapToJson()).toJson(QJsonDocument::Compact));
    return file.commit();
  }
  else
  {
    return false;
  }
}

bool FileHashUrlTable::loadMap(const QString &filepath)
{
  if(filepath.isEmpty())
    return false;
  QFile file(filepath);
  if(file.open(QIODevice::ReadOnly))
  {
    QJsonParseError error;
    QJsonArray array = QJsonDocument::fromJson(file.readAll(),&error).array();
    file.close();
    if(error.error==QJsonParseError::NoError)
    {
      loadMapFromJson(array);
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

FileHashUrlTable& FileHashUrlTable::get()
{
  static FileHashUrlTable instance("");
  return instance;
}

QByteArray FileHashUrlTable::hash(QIODevice *device)
{
  QCryptographicHash hash(QCryptographicHash::Md5);
  hash.addData(device);
  return hash.result();
}

void FileHashUrlTable::insert(const QByteArray& hash, const QString& url)
{
  m_hashUrlMap.insert(hash,url);
  saveMap(m_filepath);
}

bool FileHashUrlTable::contains(const QByteArray &hash) const
{
  return m_hashUrlMap.contains(hash);
}

const QString FileHashUrlTable::operator[](const QByteArray &hash) const
{
  return m_hashUrlMap[hash];
}
