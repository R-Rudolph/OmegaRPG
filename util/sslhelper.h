#ifndef SSLHELPER_H
#define SSLHELPER_H

#include <QSslKey>
#include <QSslCertificate>
#include <QSaveFile>

typedef struct evp_pkey_st EVP_PKEY;
typedef struct X509_name_st X509_NAME;

class SslHelper
{
  static EVP_PKEY* getEcdsaKey();
  static void addToName(X509_NAME * name, const QString& field, const QString& value);
public:
  /*
  enum GenerationError
  {
    ERROR_NONE,
    ERROR_MEMORY,
    ERROR_PEM_WRITE_PRIVATE_KEY,
    ERROR_PEM_WRITE_CERTIFICATE,
    ERROR_CERTIFICATE,
    ERROR_PRIVATE_KEY,
  };
  */
  enum GenerationError
  {
    ErrorNone,
    ErrorUnknown,
    ErrorKey,
    ErrorCertificate,
    ErrorWritePrivateKey,
    ErrorMemory
  };
  static GenerationError generateCredentials(QSslCertificate& certificate,
                                             QSslKey& key,
                                             const QString& country="DE",
                                             const QString& organization="",
                                             const QString& commonName="My Name",
                                             const QString& organizationalUnit="",
                                             const QString& locality="",
                                             const QString& state="",
                                             long validFrom=0,
                                             long validUntil=31536000L);
  static GenerationError generateTemporaryCredentials(QSslCertificate& certificate,
                                                      QSslKey& key);
  static bool saveKey(const QSslKey& key, const QString& filename);
  static bool saveCertificate(const QSslCertificate& certificate, const QString& filename);
  static QSslKey loadKey(QIODevice* device, QSsl::KeyType type=QSsl::PrivateKey, const QByteArray& passPhrase=QByteArray());
  static QSslKey loadKey(const QByteArray& data, QSsl::KeyType type=QSsl::PrivateKey, const QByteArray& passPhrase=QByteArray());
};

#endif // SSLHELPER_H
