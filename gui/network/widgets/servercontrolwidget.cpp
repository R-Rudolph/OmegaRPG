#include "servercontrolwidget.h"
#include "gui/network/dialogs/credentialsnotfounddialog.h"

ServerControlWidget::ServerControlWidget(QWidget *parent) : QWidget(parent)
{
  setWindowTitle("OmegaRPG Server");
  settings = new orpg::ServerSettings(this);
  settings->load();
  serverThread = new QThread(this);
  server = new orpg::NetworkServer(this,*settings);
  //server->moveToThread(serverThread);
  serverThread->start();
  mainLayout = new QHBoxLayout(this);
  settingsWidget = new QWidget();
  statusWidget = new QWidget();
  mainLayout->setMargin(0);
  mainLayout->addWidget(settingsWidget);
  mainLayout->addWidget(statusWidget);
  statusWidget->hide();
  settingsWidget->show();
  //settings layout
  layout = new QVBoxLayout(settingsWidget);
  settingsTabs = new QTabWidget();
  customWidget = new QWidget();
  customLayout = new QFormLayout(customWidget);
  advancedWidget = new QWidget();
  advancedLayout = new QFormLayout(advancedWidget);
  certificateWidget = new QWidget();
  certificateLayout = new QFormLayout(certificateWidget);
  settingsTabs->addTab(customWidget,"Custom");
  settingsTabs->addTab(advancedWidget,"Advanced");
  settingsTabs->addTab(certificateWidget,"Certificate");
  //init widgets
  //custom
  nameEdit = new QLineEdit();
  nameEdit->setToolTip("This name is displayed when connecting to your server.");
  customLayout->addRow("Server Name:",nameEdit);
  //advanced
  portEdit = new QSpinBox();
  metaServerCheckbox = new QCheckBox();
  domainEdit = new QLineEdit();
  metaServerUrl = new QLineEdit();
  adminPasswordEdit = new QLineEdit();
  adminPasswordEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
  domainEdit->setToolTip("The DNS address under which the server can be found. Leave empty if unsure.");
  metaServerCheckbox->setToolTip("Check if you want to register your server at the meta server for public access.");
  metaServerUrl->setToolTip("Additional Meta Servers, divided by spaces, can be inserted here.");
  adminPasswordEdit->setToolTip("The password required to achieve admin status on the server. Leave empty to prevent becoming admin.");
  portEdit->setToolTip("The TCP port the server uses. Default port is "+QString::number(Global::standardPort()));
  portEdit->setMinimum(1);
  portEdit->setMaximum(65535);
  advancedLayout->addRow("Port:",portEdit);
  advancedLayout->addRow("Register Server:",metaServerCheckbox);
  advancedLayout->addRow("Domain:",domainEdit);
  advancedLayout->addRow("Meta Server Address:",metaServerUrl);
  advancedLayout->addRow("Admin Password:",adminPasswordEdit);
  //certificate
  generateCertificateButton = new QPushButton("Generate Certificate");
  viewCertificateButton = new QPushButton("View Certificate");
  keySelect = new FileSelectWidget(true);
  certSelect = new FileSelectWidget(true);
  certificateLayout->addRow("Certificate File:",certSelect);
  certificateLayout->addRow("Private Key File:",keySelect);
  certificateLayout->addRow("",viewCertificateButton);
  certificateLayout->addRow("",generateCertificateButton);
  //button
  startServer = new QPushButton("Launch Server");
  //insert into layout
  layout->addWidget(settingsTabs,1);
  layout->addWidget(startServer,0);
  //connect signals
  connect(startServer,&QPushButton::clicked,this,&ServerControlWidget::startServerClicked);
  connect(metaServerCheckbox,&QCheckBox::toggled,this,&ServerControlWidget::notifyChecked);
  //fill in data
  nameEdit->setText(settings->serverName);
  portEdit->setValue(settings->port);
  domainEdit->setText(settings->serverUrl);
  metaServerCheckbox->setChecked(settings->notifyMetaServer);
  metaServerUrl->setText(settings->getCustomMetaServerString());
  adminPasswordEdit->setText(settings->getAdminPassword());
  keySelect->setText(settings->getPrivateKeyFilepath());
  certSelect->setText(settings->getCertificateFilepath());

  //status layout
  layout2 = new QFormLayout(statusWidget);
  //init widgets
  roomsView = new QTreeWidget(this);
  roomsView->setHeaderLabels(QStringList({"Name","Players"}));
  roomsView->setRootIsDecorated(false);
  stopServer = new QPushButton("Terminate Server");
  //insert into layout
  layout2->addWidget(roomsView);
  layout2->addWidget(stopServer);
  //connect signals
  connect(stopServer,&QPushButton::clicked,this,&ServerControlWidget::stopServerClicked);
  connect(generateCertificateButton,&QPushButton::clicked,this,&ServerControlWidget::generateCertificateClicked);
  connect(viewCertificateButton,&QPushButton::clicked,this,&ServerControlWidget::viewCertificateClicked);
  connect(server,&orpg::NetworkServer::roomsChanged,this,&ServerControlWidget::roomsChanged);
  //
  connect(this,&ServerControlWidget::closeServer,server,&orpg::NetworkServer::closeServer);
  connect(this,&ServerControlWidget::openServer,server,&orpg::NetworkServer::openServer);
  connect(this,&ServerControlWidget::setServerSettings,server,&orpg::NetworkServer::setSettings);
  connect(server,&orpg::NetworkServer::startStatus,this,&ServerControlWidget::serverStartStatus);
}

ServerControlWidget::~ServerControlWidget()
{
  serverThread->quit();
  serverThread->wait();
}

bool ServerControlWidget::restoreGeometry(const QByteArray &geometry)
{
  bool result = QWidget::restoreGeometry(geometry);
  return result;
}

void ServerControlWidget::closeEvent(QCloseEvent *event)
{
  lastSize = QWidget::size();
  lastPosition = QWidget::pos();
  event->accept();
}

void ServerControlWidget::show()
{
  QWidget::show();
  if(lastSize.isValid())
  {
    resize(QSize(0,0));
    move(lastPosition);
    resize(lastSize);
  }
}

void ServerControlWidget::startServerClicked()
{
  settings->serverName = nameEdit->text();
  settings->serverUrl = domainEdit->text();
  settings->port = portEdit->value();
  settings->notifyMetaServer = metaServerCheckbox->isChecked();
  settings->setCustomMetaServers(metaServerUrl->text());
  settings->setPrivateKeyFilepath(keySelect->text());
  settings->setCertificateFilepath(certSelect->text());
  settings->setAdminPassword(adminPasswordEdit->text());
  settings->save();
  orpg::ServerSettings::CredentialLoadingError error = settings->loadCredentials();
  if(error != orpg::ServerSettings::EverythingOk)
  {
    if(error == (error & (orpg::ServerSettings::KeyFileDoesNotExist | orpg::ServerSettings::CertFileDoesNotExist)))
    {
      CredentialsNotFoundDialog dialog(this);
      dialog.exec();
      switch(dialog.getValue())
      {
        case CredentialsNotFoundDialog::Cancel:
          return;
        case CredentialsNotFoundDialog::NewCertificate:
        {
          CertificateGenerationDialog dialog(this);
          dialog.setPaths(settings->getPrivateKeyFilepath(),settings->getCertificateFilepath());
          if(!dialog.exec())
            return;
          settings->setPrivateKeyFilepath(dialog.getKeyPath());
          settings->setCertificateFilepath(dialog.getCertPath());
          keySelect->setText(dialog.getKeyPath());
          certSelect->setText(dialog.getCertPath());
          settings->privateKey = dialog.getKey();
          QList<QSslCertificate> certList;
          certList.prepend(dialog.getCertificate());
          settings->certificateChain = certList;
          settings->save();
          break;
        }
        case CredentialsNotFoundDialog::TemporaryCertificate:
        {
          QSslCertificate cert;
          QSslKey key;
          SslHelper::generateTemporaryCredentials(cert,key);
          settings->privateKey = key;
          QList<QSslCertificate> certList;
          certList.prepend(cert);
          settings->certificateChain = certList;
          break;
        }
        default:
          break;
      }
    }
    else
    {
      QString keyError;
      QString certError;
      switch(error & (orpg::ServerSettings::KeyFileDoesNotExist|orpg::ServerSettings::KeyFileInvalid|orpg::ServerSettings::KeyFileNotOpened))
      {
        case orpg::ServerSettings::KeyFileDoesNotExist:
          keyError = "File does not exist";
          break;
        case orpg::ServerSettings::KeyFileInvalid:
          keyError = "File is invalid";
          break;
        case orpg::ServerSettings::KeyFileNotOpened:
          keyError = "Could not open file";
          break;
        default:
          keyError = "Ok";
      }
      switch(error & (orpg::ServerSettings::CertFileDoesNotExist|orpg::ServerSettings::CertFileInvalid|orpg::ServerSettings::CertFileNotOpened))
      {
        case orpg::ServerSettings::CertFileDoesNotExist:
          certError = "File does not exist";
          break;
        case orpg::ServerSettings::CertFileInvalid:
          certError = "File is invalid";
          break;
        case orpg::ServerSettings::CertFileNotOpened:
          certError = "Could not open file";
          break;
        default:
          certError = "Ok";
      }
      QMessageBox::warning(this,"Server Error","Could not load certificate or private key.\nCertificate: "+certError+"\nKey: "+keyError);
      return;
    }
  }
  emit setServerSettings(*settings);
  emit openServer();
}

void ServerControlWidget::stopServerClicked()
{
  emit closeServer();
  statusWidget->hide();
  settingsWidget->show();
}

void ServerControlWidget::notifyChecked(bool value)
{
  metaServerUrl->setDisabled(!value);
  domainEdit->setDisabled(!value);
}

void ServerControlWidget::generateCertificateClicked()
{
  CertificateGenerationDialog dialog(this);
  dialog.setPaths(keySelect->text(),certSelect->text());
  if(!dialog.exec())
    return;
  keySelect->setText(dialog.getKeyPath());
  certSelect->setText(dialog.getCertPath());
}

void ServerControlWidget::viewCertificateClicked()
{
  settings->setCertificateFilepath(certSelect->text());
  orpg::ServerSettings::CredentialLoadingError error = settings->loadCert();
  if(error!=orpg::ServerSettings::EverythingOk)
  {
    QString certError;
    switch(error & (orpg::ServerSettings::CertFileDoesNotExist|orpg::ServerSettings::CertFileInvalid|orpg::ServerSettings::CertFileNotOpened))
    {
      case orpg::ServerSettings::CertFileDoesNotExist:
        certError = "File does not exist";
        break;
      case orpg::ServerSettings::CertFileInvalid:
        certError = "File is invalid";
        break;
      case orpg::ServerSettings::CertFileNotOpened:
        certError = "Could not open file";
        break;
      default:
        certError = "Ok";
    }
    QMessageBox::warning(this,"Certificate Error","Could not load certificate.\n"+certError);
    return;
  }
  CertificateViewer::call(settings->certificateChain.first(),this);
}

void ServerControlWidget::roomsChanged(const QMap<int, orpg::Room*>& map)
{
  roomsView->clear();
  foreach(orpg::Room* room,map)
  {
    QTreeWidgetItem* item = new QTreeWidgetItem(QStringList{room->_name,QString::number(room->_numPlayers)});
    roomsView->addTopLevelItem(item);
  }
}

void ServerControlWidget::serverStartStatus(bool success)
{
  if(success)
  {
    settingsWidget->hide();
    statusWidget->show();
  }
  else
  {
    QMessageBox::warning(this,"Server Error","Could not bind port for server.");
  }
}
