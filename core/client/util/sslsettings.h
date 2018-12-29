#ifndef SSLSETTINGS_H
#define SSLSETTINGS_H

#include <QObject>
#include <QSslCertificate>
#include <QSslError>
#include <QSet>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSaveFile>
#include <QFile>

#include "core/multi/util/global.h"

class SslSettings : public QObject
{
  Q_OBJECT
  static SslSettings* singleton;
  QSet<QSslError> permanentExceptions;
  QSet<QSslError> temporaryExceptions;
  QString filename;
  void load();
  void save();
  bool parse(const QJsonObject& mainObject);
  QJsonObject toJson();
public:
  explicit SslSettings(const QString& filename="", QObject *parent = 0);
  ~SslSettings();
  void addPermanentException(const QSslError& error);
  void addTemporaryException(const QSslError& error);
  void removePermanentException(const QSslError& error);
  void removeTemporaryException(const QSslError& error);
  void setFilename(const QString& filename);
  QSet<QSslError> allExceptions();
  bool isInExceptions(const QSslError& error);
  static SslSettings* get();
  QSet<QSslError> getPermanentExceptions() const;
  QSet<QSslError> getTemporaryExceptions() const;
signals:
  void permanentExceptionAdded(const QSslError& error);
  void permanentExceptionRemoved(const QSslError& error);
  void temporaryExceptionAdded(const QSslError& error);
  void temporaryExceptionRemoved(const QSslError& error);
  void exceptionsChanged();

public slots:
};

#endif // SSLSETTINGS_H
