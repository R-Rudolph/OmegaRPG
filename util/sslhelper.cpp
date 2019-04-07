#include "sslhelper.h"

#include <openssl/rsa.h>
#include <openssl/ssl.h>
#include <openssl/ecdsa.h>
#include <openssl/x509v3.h>
#include <openssl/x509.h>

void SslHelper::addToName(X509_NAME * name, const QString& field, const QString& value)
//void SslHelper::addToName(const QString& field, const QString& value)
{
  if(!value.isEmpty())
  {
    X509_NAME_add_entry_by_txt(name, field.toStdString().c_str(),  MBSTRING_ASC, (unsigned char *)value.toStdString().c_str(), -1, -1, 0);
  }
}

EVP_PKEY* SslHelper::getEcdsaKey()
{
  EVP_PKEY* pkey = EVP_PKEY_new();
  {
    //generate the key ECDSA)
    EC_KEY * ecc = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
    if(!ecc)
    {
        // OpenSSL Error. Use `ERR_peek_last_error_line` to find out more.
        return nullptr;
    }

    EC_KEY_set_asn1_flag(ecc, OPENSSL_EC_NAMED_CURVE);
    if(EC_KEY_generate_key(ecc) < 0)
    {
        // OpenSSL Error. Use `ERR_peek_last_error_line` to find out more.
        return nullptr;
    }

    if(EVP_PKEY_assign_EC_KEY(pkey, ecc) < 0)
    {
        // OpenSSL Error. Use `ERR_peek_last_error_line` to find out more.
        return nullptr;
    }
  }
  return pkey;
}

SslHelper::GenerationError SslHelper::generateCredentials(QSslCertificate& certificate,
                                                          QSslKey& key,
                                                          const QString& country,
                                                          const QString& organization,
                                                          const QString& commonName,
                                                          const QString& organizationalUnit,
                                                          const QString& locality,
                                                          const QString& state,
                                                          long validFrom,
                                                          long validUntil)
{
  //https://dev.to/ecnepsnai/pragmatically-generating-a-self-signed-certificate-and-private-key-usingopenssl
  OPENSSL_init_ssl(0, nullptr);
  OPENSSL_init_crypto(0, nullptr);

  X509 * cert;
  EVP_PKEY* pkey = getEcdsaKey();
  if(pkey==nullptr)
    return ErrorKey;

  //generate the certificate
  cert = X509_new();
  X509_set_pubkey(cert, pkey);

  //set serial number
  unsigned long random_serial_number = 0x1337;
  ASN1_INTEGER_set(X509_get_serialNumber(cert), random_serial_number);
  //set validity date range
  X509_gmtime_adj((ASN1_TIME *)X509_get0_notBefore(cert), validFrom);
  X509_gmtime_adj((ASN1_TIME *)X509_get0_notAfter(cert), validUntil);
  //X509_gmtime_adj((ASN1_TIME *)cert->cert_info->validity->notBefore, validFrom);
  //X509_gmtime_adj((ASN1_TIME *)cert->cert_info->validity->notAfter, validUntil);
  //set name
  X509_NAME * name;
  name = X509_get_subject_name(cert);
  addToName(name,"CN",commonName);
  addToName(name,"OU",organizationalUnit);
  addToName(name,"O",organization);
  addToName(name,"L",locality);
  addToName(name,"ST",state);
  addToName(name,"C",country);
  X509_set_issuer_name(cert, name);
  //TODO: Subject Alternative Name

  //sign certificate
  if (X509_sign(cert, pkey, EVP_sha256()) < 0)
  {
      // OpenSSL Error. Use `ERR_peek_last_error_line` to find out more.
      EVP_PKEY_free(pkey);
      X509_free(cert);
      return ErrorCertificate;
  }
  //write private key
  {
    BIO* bio;
    bio = BIO_new(BIO_s_mem());
    if(PEM_write_bio_PrivateKey(bio, pkey, nullptr, nullptr, 0, nullptr, nullptr) != 1)
    {
      BIO_free(bio);
      EVP_PKEY_free(pkey);
      X509_free(cert);
      return ErrorWritePrivateKey;
    }
    char* buffer = nullptr;
    long bufferSize = BIO_get_mem_data(bio, &buffer);
    if(buffer==nullptr)
    {
      EVP_PKEY_free(pkey);
      X509_free(cert);
      BIO_free_all(bio);
      return ErrorMemory;
    }
    QByteArray qbuffer(buffer,bufferSize);
    key = QSslKey(qbuffer,QSsl::Ec);
    EVP_PKEY_free(pkey);
    BIO_free_all(bio);
    if(key.isNull())
    {
      X509_free(cert);
      return ErrorKey;
    }
  }
  //write certificate
  {
    BIO* bio;
    bio = BIO_new(BIO_s_mem());
    if(PEM_write_bio_X509(bio, cert) != 1)
    {
      BIO_free(bio);
      X509_free(cert);
      return ErrorWritePrivateKey;
    }
    char* buffer = nullptr;
    long bufferSize = BIO_get_mem_data(bio, &buffer);
    if(buffer==nullptr)
    {
      X509_free(cert);
      BIO_free_all(bio);
      return ErrorMemory;
    }
    QByteArray qbuffer(buffer,bufferSize);
    certificate = QSslCertificate(qbuffer);
    X509_free(cert);
    BIO_free_all(bio);
    if(certificate.isNull())
    {
      return ErrorCertificate;
    }
  }
  return ErrorNone;
}

SslHelper::GenerationError SslHelper::generateTemporaryCredentials(QSslCertificate& certificate, QSslKey& key)
{
  return generateCredentials(certificate,key,"","","Temporary Certificate");
}

bool SslHelper::saveKey(const QSslKey &key, const QString &filename)
{
  QSaveFile file(filename);
  if(file.open(QIODevice::WriteOnly))
  {
    file.write(key.toPem());
    return file.commit();
  }
  return false;
}

bool SslHelper::saveCertificate(const QSslCertificate &certificate, const QString &filename)
{
  QSaveFile file(filename);
  if(file.open(QIODevice::WriteOnly))
  {
    file.write(certificate.toPem());
    return file.commit();
  }
  return false;
}

QSslKey SslHelper::loadKey(QIODevice* device, QSsl::KeyType type, const QByteArray& passPhrase)
{
  if(device->isSequential())
  {
    return loadKey(device->readAll(),type,passPhrase);
  }
  else
  {
    QList<QSsl::KeyAlgorithm> algorithms({QSsl::Rsa,QSsl::Dsa,QSsl::Ec,QSsl::Opaque});
    QList<QSsl::EncodingFormat> encodings({QSsl::Pem,QSsl::Der});
    qint64 startPos = device->pos();
    foreach(QSsl::KeyAlgorithm algorithm, algorithms)
    {
      foreach(QSsl::EncodingFormat encoding, encodings)
      {
        device->seek(startPos);
        QSslKey key(device,algorithm,encoding,type,passPhrase);
        if(!key.isNull())
          return key;
      }
    }
  }
  return QSslKey();
}

QSslKey SslHelper::loadKey(const QByteArray& data, QSsl::KeyType type, const QByteArray& passPhrase)
{
  QList<QSsl::KeyAlgorithm> algorithms({QSsl::Rsa,QSsl::Dsa,QSsl::Ec,QSsl::Opaque});
  QList<QSsl::EncodingFormat> encodings({QSsl::Pem,QSsl::Der});
  foreach(QSsl::KeyAlgorithm algorithm, algorithms)
  {
    foreach(QSsl::EncodingFormat encoding, encodings)
    {
      QSslKey key(data,algorithm,encoding,type,passPhrase);
      if(!key.isNull())
        return key;
    }
  }
  return QSslKey();
}
