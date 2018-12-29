#include "sslsettings.h"

SslSettings* SslSettings::singleton = NULL;

QSet<QSslError> SslSettings::getPermanentExceptions() const
{
  return permanentExceptions;
}

QSet<QSslError> SslSettings::getTemporaryExceptions() const
{
  return temporaryExceptions;
}

void SslSettings::load()
{
  QFile file(filename);
  if(file.open(QIODevice::ReadOnly))
  {
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(),&error);
    if(error.error != QJsonParseError::NoError)
      return;
    parse(doc.object());
  }
}

void SslSettings::save()
{
  QSaveFile file(filename);
  if(file.open(QIODevice::WriteOnly))
  {
    QJsonDocument doc(toJson());
    file.write(doc.toJson());
    file.commit();
  }
}

bool SslSettings::parse(const QJsonObject &mainObject)
{
  if(mainObject["type"].toString()!="OmegaRPG SSL Settings File")
    return false;
  permanentExceptions.clear();
  foreach(const QJsonValue& errorValue, mainObject["exceptions"].toArray())
  {
    QJsonObject errorObject = errorValue.toObject();
    QList<QSslCertificate> certs = QSslCertificate::fromData(QByteArray::fromBase64(errorObject["cert"].toString().toLatin1()),QSsl::Pem);
    if(certs.size()<1)
      continue;
    QSslError error((QSslError::SslError)errorObject["error"].toInt(),certs.at(0));
    permanentExceptions.insert(error);
  }
  return true;
}

QJsonObject SslSettings::toJson()
{
  QJsonObject mainObject;
  mainObject.insert("type","OmegaRPG SSL Settings File");
  QJsonArray exceptions;
  foreach(QSslError error, permanentExceptions)
  {
    QJsonObject errorObject;
    errorObject.insert("error",error.error());
    errorObject.insert("cert",QString::fromLatin1(error.certificate().toPem().toBase64()));
    exceptions.push_back(errorObject);
  }
  mainObject.insert("exceptions",exceptions);
  return mainObject;
}

SslSettings::SslSettings(const QString& filename, QObject *parent) : QObject(parent)
{
  setFilename(filename);
  load();
}

SslSettings::~SslSettings()
{
  save();
}

void SslSettings::addPermanentException(const QSslError &error)
{
  if(!permanentExceptions.contains(error))
  {
    permanentExceptions.insert(error);
    emit permanentExceptionAdded(error);
    emit exceptionsChanged();
    save();
  }
}

void SslSettings::addTemporaryException(const QSslError &error)
{
  if(!temporaryExceptions.contains(error))
  {
    temporaryExceptions.insert(error);
    emit temporaryExceptionAdded(error);
    emit exceptionsChanged();
  }
}

void SslSettings::removePermanentException(const QSslError &error)
{
  if(permanentExceptions.contains(error))
  {
    permanentExceptions.remove(error);
    emit permanentExceptionRemoved(error);
    emit exceptionsChanged();
    save();
  }
}

void SslSettings::removeTemporaryException(const QSslError &error)
{
  if(temporaryExceptions.contains(error))
  {
    temporaryExceptions.remove(error);
    emit temporaryExceptionRemoved(error);
    emit exceptionsChanged();
  }
}

void SslSettings::setFilename(const QString &filename)
{
  if(filename.isEmpty())
    this->filename = Global::sslSettingsFilepath();
  else
    this->filename = filename;
}

QSet<QSslError> SslSettings::allExceptions()
{
  QSet<QSslError> result;
  result += permanentExceptions;
  result += temporaryExceptions;
  return result;
}

bool SslSettings::isInExceptions(const QSslError &error)
{
  return permanentExceptions.contains(error) || temporaryExceptions.contains(error);
}

SslSettings *SslSettings::get()
{
  if(SslSettings::singleton==NULL)
    SslSettings::singleton = new SslSettings();
  return SslSettings::singleton;
}
