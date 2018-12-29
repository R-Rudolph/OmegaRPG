#ifndef ROOMINFO_H
#define ROOMINFO_H

#include <QString>
#include <QJsonObject>
#include<QDateTime>

class RoomInfo
{
public:
  static const QString idField;
  static const QString nameField;
  static const QString hasPasswordField;
  static const QString playerNumberField;
  static const QString timeField;
  static const QString timeFormat;
private:
  int _id;
  QString _name;
  QDateTime _time;
  int _numPlayers;
  bool _hasPassword;
  bool _valid;
public:
  RoomInfo(const QString &name, bool hasPassword, int id, int numPlayers, QDateTime time);
  RoomInfo(const QJsonObject& object);
  RoomInfo();

  QJsonObject toJson() const;
  bool isValid() const;

  int id() const;
  void setId(int id);
  QString name() const;
  void setName(const QString& name);
  int numPlayers() const;
  void setNumPlayers(int numPlayers);
  bool hasPassword() const;
  void setHasPassword(bool hasPassword);
  QString uid() const;
  QDateTime time() const;
};

#endif // ROOMINFO_H
