#ifndef SERVERCONTROLWIDGET_H
#define SERVERCONTROLWIDGET_H

#include <QWidget>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QCloseEvent>
#include <QPushButton>
#include <QMessageBox>
#include <QTreeWidget>
#include <QTabWidget>
#include <QThread>
#include "core/server/serversettings.h"
#include "core/server/networkserver.h"
#include "core/multi/util/global.h"
#include "gui/util/globalgui.h"
#include "dialogs/certificategenerationdialog.h"
#include "widgets/fileselectwidget.h"
#include "dialogs/certificateviewer.h"

class ServerControlWidget : public QWidget
{
  Q_OBJECT
  QSize lastSize;
  QPoint lastPosition;
  QThread* serverThread;
  orpg::ServerSettings* settings;
  //top level stuff
  QHBoxLayout* mainLayout;
  QWidget* settingsWidget;
  QWidget* statusWidget;
  //server settings stuff
  QVBoxLayout* layout;
  QTabWidget* settingsTabs;
  //costumize options
  QWidget* customWidget;
  QFormLayout* customLayout;
  QLineEdit* nameEdit;
  //advanced options
  QWidget* advancedWidget;
  QFormLayout* advancedLayout;
  QSpinBox* portEdit;
  QCheckBox* metaServerCheckbox;
  QLineEdit* domainEdit;
  QLineEdit* metaServerUrl;
  QLineEdit* adminPasswordEdit;
  //certificate options
  QWidget* certificateWidget;
  QFormLayout* certificateLayout;
  QPushButton* viewCertificateButton;
  QPushButton* generateCertificateButton;
  FileSelectWidget* keySelect;
  FileSelectWidget* certSelect;
  QPushButton* startServer;
  //server status stuff
  QFormLayout* layout2;
  QTreeWidget* roomsView;
  QPushButton* stopServer;

  orpg::NetworkServer* server;


public:
  explicit ServerControlWidget(QWidget *parent = 0);
  ~ServerControlWidget();
  bool restoreGeometry(const QByteArray &geometry);
protected:
  void closeEvent(QCloseEvent* event);
signals:
  void setServerSettings(const orpg::ServerSettings& settings);
  void openServer();
  void closeServer();
public slots:
  void show();
private slots:
  void startServerClicked();
  void stopServerClicked();
  void notifyChecked(bool value);
  void generateCertificateClicked();
  void viewCertificateClicked();
  void roomsChanged(const QMap<int,orpg::Room*>& map);
  void serverStartStatus(bool success);
};

#endif // SERVERCONTROLWIDGET_H
