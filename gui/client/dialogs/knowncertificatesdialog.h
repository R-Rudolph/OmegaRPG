#ifndef KNOWNCERTIFICATESDIALOG_H
#define KNOWNCERTIFICATESDIALOG_H

#include <QObject>
#include <QWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QAction>
#include <QPushButton>
#include <QSslCertificate>
#include <QSslError>
#include <QMap>
#include <QMultiMap>
#include <QMessageBox>
#include <QResizeEvent>

#include "core/client/util/sslsettings.h"
#include "dialogs/certificateviewer.h"

class KnownCertificatesWindow : public QWidget
{
  Q_OBJECT
  QSize lastSize;
  QPoint lastPosition;
  SslSettings* ssl;
  QMap<QTreeWidgetItem*,QSslCertificate> tempCertificateMap;
  QMap<QTreeWidgetItem*,QSslCertificate> permCertificateMap;
  QMap<QTreeWidgetItem*, QSslError::SslError> errorMap;
  QMultiMap<QSslCertificate,QSslError::SslError> permCertErrorMap;
  QMultiMap<QSslCertificate,QSslError::SslError> tempCertErrorMap;
  QTreeWidgetItem* currentCertItem;
  QTreeWidgetItem* currentErrorItem;

  QTreeWidgetItem* tempCertItem;
  QTreeWidgetItem* permCertItem;

  QHBoxLayout* mainLayout;
  QVBoxLayout* rightLayout;
  QTreeWidget* certificateList;
  QTreeWidget* errorList;
  QPushButton* closeButton;

  QTreeWidgetItem* getPermItem(const QSslCertificate& cert);
  QTreeWidgetItem* getTempItem(const QSslCertificate& cert);
  QStringList getDescriptiveText(const QSslCertificate& cert);
  QString getDescriptiveText(const QSslError& error);
  QSslCertificate getCertificate(QTreeWidgetItem* item);
public:
  explicit KnownCertificatesWindow(QWidget *parent = 0);
  bool restoreGeometry(const QByteArray &geometry);
protected:
  void closeEvent(QCloseEvent* event);
signals:
public slots:
  void show();
private slots:
  void updateCurrentErrors();
  //context menues
  void certificatesContextMenu(const QPoint & pos);
  void errorContextMenu(const QPoint & pos);
  void viewCertificate();
  void removeCertificate();
  void removeError();
  //react to ssl settings
  void permanentExceptionAdded(const QSslError& error);
  void permanentExceptionRemoved(const QSslError& error);
  void temporaryExceptionAdded(const QSslError& error);
  void temporaryExceptionRemoved(const QSslError& error);

};

#endif // KNOWNCERTIFICATESDIALOG_H
