#include "omegarpgmainwindow.h"
#include "objects/nightmode.h"
#include "core/client/alias/aliaslib.h"
#include "core/client/util/dice.h"

//OmegaRpgMainWindow::OmegaRpgMainWindow(QWidget *parent) : QMainWindow(parent)

void OmegaRpgMainWindow::setInitiativeMenu(bool value)
{
  initiativeShown = value;
  if(initiativeShown)
    initiativeAction->setIcon(GlobalGUI::iconActive());
  else
    initiativeAction->setIcon(QIcon());
  map->showInitiative(value);
}

void OmegaRpgMainWindow::restoreWindow()
{
  QSettings windowSettings(Global::windowSettingsFilepath(),QSettings::IniFormat);
  if(windowSettings.contains("mainGeometry"))
    restoreGeometry(windowSettings.value("mainGeometry").toByteArray());
  if(windowSettings.contains("mainState"))
    restoreState(windowSettings.value("mainState").toByteArray());

  if(windowSettings.contains("msGeometry"))
    mainSplitter->restoreGeometry(windowSettings.value("msGeometry").toByteArray());
  if(windowSettings.contains("msState"))
    mainSplitter->restoreState(windowSettings.value("msState").toByteArray());

  if(windowSettings.contains("bsGeometry"))
    bottomSplitter->restoreGeometry(windowSettings.value("bsGeometry").toByteArray());
  if(windowSettings.contains("bsState"))
    bottomSplitter->restoreState(windowSettings.value("bsState").toByteArray());

  if(windowSettings.contains("chatGeometry"))
    chat->getSplitter()->restoreGeometry(windowSettings.value("chatGeometry").toByteArray());
  if(windowSettings.contains("chatState"))
    chat->getSplitter()->restoreState(windowSettings.value("chatState").toByteArray());

  if(windowSettings.contains("logGeometry"))
    logWindow->restoreGeometry(windowSettings.value("logGeometry").toByteArray());
  if(windowSettings.contains("logState"))
    logWindow->getSplitter()->restoreState(windowSettings.value("logState").toByteArray());

  if(windowSettings.contains("aliasGeometry"))
    aliasWindow->restoreGeometry(windowSettings.value("aliasGeometry").toByteArray());
  if(windowSettings.contains("aliasState"))
    aliasWindow->getSplitter()->restoreState(windowSettings.value("aliasState").toByteArray());
}

void OmegaRpgMainWindow::saveWindow()
{
  QSettings windowSettings(Global::windowSettingsFilepath(),QSettings::IniFormat);
  windowSettings.setValue("mainGeometry",saveGeometry());
  windowSettings.setValue("mainState",saveState());
  windowSettings.setValue("msGeometry",mainSplitter->saveGeometry());
  windowSettings.setValue("msState",mainSplitter->saveState());
  windowSettings.setValue("bsGeometry",bottomSplitter->saveGeometry());
  windowSettings.setValue("bsState",bottomSplitter->saveState());
  windowSettings.setValue("chatGeometry",chat->getSplitter()->saveGeometry());
  windowSettings.setValue("chatState",chat->getSplitter()->saveState());
  windowSettings.setValue("logGeometry",logWindow->saveGeometry());
  windowSettings.setValue("logState",logWindow->getSplitter()->saveState());
  windowSettings.setValue("aliasGeometry",aliasWindow->saveGeometry());
  windowSettings.setValue("aliasState",aliasWindow->getSplitter()->saveState());
}

OmegaRpgMainWindow::OmegaRpgMainWindow()
{
  logWindow = new LogViewerWindow(this);

  layout = new QVBoxLayout();
  layout->setContentsMargins(10,0,10,5);
  playerListSplitter = new QSplitter();
  mainSplitter = new QSplitter();
  bottomSplitter = new QSplitter();
  //playerAliasSplitter = new QSplitter(Qt::Vertical,this);

  chat = new ChatAreaWidget(this);

  mainWindowWidget = new NightModeOverlay(this);
  QWidget* wrappedWidget = new QWidget(mainWindowWidget);
  mainWindowWidget->setWidget(wrappedWidget);
  wrappedWidget->setLayout(layout);

  pluginWindow_ = new PluginLibraryWidget();

  map = new MapWidget(this);
  //player list
  playerList = new PlayerListAreaWidget(this);

  bottomSplitter->addWidget(playerList);
  bottomSplitter->addWidget(chat);
  bottomSplitter->setSizes(QList<int>({200,600}));

  mainSplitter->addWidget(map);
  mainSplitter->addWidget(bottomSplitter);
  mainSplitter->setOrientation(Qt::Vertical);
  mainSplitter->setStretchFactor(0,1);
  mainSplitter->setStretchFactor(1,10);

  bottomSplitter->setOrientation(Qt::Horizontal);
  bottomSplitter->setCollapsible(0,false);
  bottomSplitter->setCollapsible(1,false);
  bottomSplitter->setStretchFactor(0,1);
  bottomSplitter->setStretchFactor(1,10);

  actionConnectionMenu = new QAction("Connection",this);

  this->setMenuBar(new QMenuBar());
  menuOmegaRpg = this->menuBar()->addMenu("OmegaRPG");
  this->menuBar()->addAction(actionConnectionMenu);
  menuWindows = this->menuBar()->addMenu("Windows");
  menuRoomProperties = this->menuBar()->addMenu("Room Properties");
  menuHelp = this->menuBar()->addMenu("Help");


  actionAliasMenu = new QAction("Alias Library",this);
  actionSettingsMenu = new QAction(GlobalGUI::freedesktopSystem(),"Settings",this);

  menuOmegaRpg->addAction(actionSettingsMenu);
  QMenu* nightModeMenu = menuOmegaRpg->addMenu("Night Mode");
  nightModeMenu->addAction("Toggle",NightModeController::get(),&NightModeController::toggle);
  QMenu* overlayColorMenu = nightModeMenu->addMenu("Overlay Color");
  overlayColorMenu->addAction("Color: Grey",this,&OmegaRpgMainWindow::nightModeGrey);
  overlayColorMenu->addAction("Color: Red",this,&OmegaRpgMainWindow::nightModeRed);
  overlayColorMenu->addAction("Color: Green",this,&OmegaRpgMainWindow::nightModeGreen);
  overlayColorMenu->addAction("Color: Blue",this,&OmegaRpgMainWindow::nightModeBlue);

  menuOmegaRpg->addSeparator();
  serverWindow = new ServerControlWidget();
  menuOmegaRpg->addAction(GlobalGUI::serverIcon(),"Server",serverWindow,&ServerControlWidget::show);
  menuOmegaRpg->addSeparator();
  menuOmegaRpg->addAction(GlobalGUI::freedesktopExit(),"Exit",this,&OmegaRpgMainWindow::close);
  initiativeAction = menuWindows->addAction("Initiative Menu",this,&OmegaRpgMainWindow::toogleInitiativeMenu);
  menuWindows->addSeparator();
  menuWindows->addAction(actionAliasMenu);
  menuWindows->addAction("Logs",logWindow,&LogViewerWindow::show);
  menuWindows->addAction("Plugin Library",pluginWindow_,&PluginLibraryWidget::show);

  exportRoomPropertieseAction = menuRoomProperties->addAction(GlobalGUI::freedesktopSave(),"Export Properties",this,&OmegaRpgMainWindow::exportProperties);
  importRoomPropertieseAction = menuRoomProperties->addAction(GlobalGUI::freedesktopLoad(),"Import Properties",this,&OmegaRpgMainWindow::importProperties);

  menuHelp->addAction("Reset Dice",this,&OmegaRpgMainWindow::reseedRandom);
  menuHelp->addSeparator();
  menuHelp->addAction("About Qt",this,&OmegaRpgMainWindow::aboutQt);
  menuHelp->addAction(GlobalGUI::freedesktopAbout(),"About OmegaRPG",this,&OmegaRpgMainWindow::aboutOmegaRPG);

  playerListSplitter->addWidget(mainSplitter);
  playerListSplitter->setOrientation(Qt::Horizontal);
  layout->addWidget(playerListSplitter);
  this->setCentralWidget(mainWindowWidget);
  this->setWindowTitle("OmegaRPG");
  this->resize(800,600);

  control = new orpg::ClientControl(this);

  //other windows

  connectionWindow = new OmegaRpgConnectionWindow();
  aliasWindow = new AliasWindow(this);
  settingsWindow = new OmegaRpgSettingsWindow(this);
  knownCertificateDialog = new KnownCertificatesWindow(this);
  menuWindows->addAction(GlobalGUI::freedesktopSecurityHigh(),"SSL Exceptions",knownCertificateDialog,&KnownCertificatesWindow::show);

  sfx = new QMediaPlayer(this);
  setSoundVolume(orpg::Settings::get()->getCorrectedVolume());
  connect(orpg::Settings::get(),&orpg::Settings::soundfilePathChanged,this,&OmegaRpgMainWindow::setSound);
  setSound(orpg::Settings::get()->getSoundfilePath());

  //signals

  //action signals
  connect(actionSettingsMenu,&QAction::triggered,settingsWindow,&OmegaRpgSettingsWindow::show);
  connect(actionSettingsMenu,&QAction::triggered,settingsWindow,&QWidget::raise);
  connect(actionAliasMenu,&QAction::triggered,aliasWindow,&AliasWindow::show);
  connect(actionAliasMenu,&QAction::triggered,aliasWindow,&QWidget::raise);
  connect(actionConnectionMenu,&QAction::triggered,connectionWindow,&OmegaRpgConnectionWindow::show);
  connect(actionConnectionMenu,&QAction::triggered,connectionWindow,&QWidget::raise);

  //player list signals
  connect(control,&orpg::ClientControl::playersChanged,playerList,&PlayerListAreaWidget::setPlayers);
  connect(control,&orpg::ClientControl::roleChanged,playerList,&PlayerListAreaWidget::setClientStatus);
  connect(control,&orpg::ClientControl::roleChanged,this,&OmegaRpgMainWindow::roleChanged);
  connect(playerList,&PlayerListAreaWidget::assignStatus,control,&orpg::ClientControl::assignStatus);
  connect(playerList,&PlayerListAreaWidget::openWhisper,chat,&ChatAreaWidget::openWhisperTab);
  connect(playerList,&PlayerListAreaWidget::bootFromRoom,control,&orpg::ClientControl::kick);
  connect(playerList,&PlayerListAreaWidget::chat,chat,&ChatAreaWidget::chatPrivate);

  //settings window signals
  connect(settingsWindow,&OmegaRpgSettingsWindow::demandSettings,this,&OmegaRpgMainWindow::updateSettingsWindow);
  connect(settingsWindow,&OmegaRpgSettingsWindow::publishSettings,control,&orpg::ClientControl::publishSettings);
  connect(settingsWindow,&OmegaRpgSettingsWindow::produceSound,this,&OmegaRpgMainWindow::playSound);

  //map signals
  connect(control,&orpg::ClientControl::roleChanged,map,&MapWidget::setClientStatus);

  //connection window signals
  connect(connectionWindow,&OmegaRpgConnectionWindow::connectToHost,control,&orpg::ClientControl::connect);
  connect(connectionWindow,&OmegaRpgConnectionWindow::disconnectFromHost,control,&orpg::ClientControl::disconnectFromHost);
  connect(control,&orpg::ClientControl::roomsChanged,this->connectionWindow,&OmegaRpgConnectionWindow::setRooms);
  connect(connectionWindow,&OmegaRpgConnectionWindow::joinRoom,control,&orpg::ClientControl::joinRoom);
  connect(connectionWindow,&OmegaRpgConnectionWindow::createRoom,control,&orpg::ClientControl::createRoom);
  connect(control,&orpg::ClientControl::connected,connectionWindow,&OmegaRpgConnectionWindow::connected);
  connect(control,&orpg::ClientControl::disconnected,connectionWindow,&OmegaRpgConnectionWindow::disconnected);
  connect(connectionWindow,&OmegaRpgConnectionWindow::serverMenu,serverWindow,&ServerControlWidget::show);

  //chat signals
  connect(chat,&ChatAreaWidget::setSheetsVisible,this,&OmegaRpgMainWindow::setAliasSheetVisible);
  //connect(chat,&ChatAreaWidget::setSheetsVisible,playerList,&PlayerListAreaWidget::setAliasSheetVisible);
  connect(chat,&ChatAreaWidget::idle,control,&orpg::ClientControl::idle);
  connect(chat,&ChatAreaWidget::typed,control,&orpg::ClientControl::typing);
  connect(chat,&ChatAreaWidget::chat,control,&orpg::ClientControl::chat);
  connect(control, &orpg::ClientControl::errorMessage, chat, &ChatAreaWidget::printErrorMessage);
  connect(control, &orpg::ClientControl::statusMessage, chat, &ChatAreaWidget::printStatusMessage);
  connect(control, &orpg::ClientControl::receivedChat, chat, &ChatAreaWidget::printChatMessage);
  connect(control,&orpg::ClientControl::playersChanged,chat,&ChatAreaWidget::setPlayers);
  connect(control,&orpg::ClientControl::idChanged,chat,&ChatAreaWidget::setID);
  connect(control,&orpg::ClientControl::idChanged,playerList,&PlayerListAreaWidget::setID);
  connect(chat,&ChatAreaWidget::notify,this,&OmegaRpgMainWindow::notify);
  connect(chat,&ChatAreaWidget::inactiveChannelMessage,this,&OmegaRpgMainWindow::chatAreaNotification);

  //audio signals
  connect(orpg::Settings::get(),&orpg::Settings::correctedVolumeChanged,this,&OmegaRpgMainWindow::setSoundVolume);

  //error signals
  connect(orpg::Settings::get(),&orpg::Settings::saveFailed,this,&OmegaRpgMainWindow::settingsSaveFailed);
  connect(AliasLib::get(),&AliasLib::saveFailed,this,&OmegaRpgMainWindow::aliasSaveFailed);

  //get info
  settingsWindow->setSettings(control->getSettings());

  //splitter visuals
  playerListSplitter->setStyleSheet("QSplitter::handle:vertical"
                              "{"
                              "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
                              "stop:0 rgba(0, 0, 0, 0), "
                              "stop:0.25 rgba(0, 0, 0, 24), "
                              "stop:0.5 rgba(0, 0, 0, 32), "
                              "stop:0.75 rgba(0, 0, 0, 24), "
                              "stop:1 rgba(0, 0, 0, 0));"
                              "}"
                              "QSplitter::handle:horizontal"
                              "{"
                              "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, "
                              "stop:0 rgba(0, 0, 0, 0), "
                              "stop:0.25 rgba(0, 0, 0, 24), "
                              "stop:0.5 rgba(0, 0, 0, 32), "
                              "stop:0.75 rgba(0, 0, 0, 24), "
                              "stop:1 rgba(0, 0, 0, 0));"
                              "}");



  suppressSound = true;

  switch(orpg::Settings::get()->getLoadError())
  {
    case orpg::Settings::LOAD_JSON_ERROR:
      chat->printErrorMessage(orpg::Settings::get()->getLoadErrorString());
      break;
    case orpg::Settings::LOAD_OPEN_ERROR:
      chat->printErrorMessage(orpg::Settings::get()->getLoadErrorString());
      break;
    case orpg::Settings::LOAD_EXISTS_ERROR:
      chat->printStatusMessage(orpg::Settings::get()->getLoadErrorString());
      break;
    default:
      break;
  }

  switch(AliasLib::get()->getLoadError())
  {
    case AliasLib::LOAD_OPEN_ERROR:
    case AliasLib::LOAD_JSON_ERROR:
      chat->printErrorMessage(AliasLib::get()->getLoadErrorString());
      break;
    case AliasLib::LOAD_EXISTS_ERROR:
      chat->printStatusMessage(AliasLib::get()->getLoadErrorString());
      break;
    default:
      break;
  }

  suppressSound = false;

  restoreWindow();
  setInitiativeMenu(false);
  control->announceCurrentState();
}

void OmegaRpgMainWindow::closeEvent(QCloseEvent *event)
{
  QMessageBox msgBox(this);
  msgBox.setText("Do you want to quit OmegaRPG?");
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::Yes);
  int ret = msgBox.exec();
  switch(ret)
  {
    case QMessageBox::Yes:
    {
      if(settingsWindow->close() &&
         connectionWindow->close() &&
         logWindow->close() &&
         serverWindow->close() &&
         aliasWindow->close() &&
         pluginWindow_->close())
      {
        saveWindow();
        settingsWindow->deleteLater();
        connectionWindow->deleteLater();
        serverWindow->deleteLater();
        logWindow->deleteLater();
        pluginWindow_->deleteLater();
        event->accept();
      }
      else
      {
        event->ignore();
      }
      break;
    }
    case QMessageBox::No:
      this->show();
      event->ignore();
      break;
    default:
      event->ignore();
      break;
  }
}

void OmegaRpgMainWindow::setSoundVolume(qreal volume)
{
  sfx->setVolume(int(volume*100));
}

void OmegaRpgMainWindow::notify()
{
  if(!this->isActiveWindow() && !connectionWindow->isActiveWindow())
  {
    playSound();
    QApplication::alert(this);
  }
}

void OmegaRpgMainWindow::playSound()
{
  if(!suppressSound)
    sfx->play();
}

void OmegaRpgMainWindow::setSound(const QString&)
{
  sfx->setMedia(QUrl(orpg::Settings::get()->getSoundfilePath()));
}

void OmegaRpgMainWindow::updateSettingsWindow()
{
  //aliasWindow->applyChanges(true);
}

void OmegaRpgMainWindow::nightModeGrey()
{
  NightModeController::get()->setOverlayColor(QColor(0,0,0,128));
}

void OmegaRpgMainWindow::nightModeRed()
{
  NightModeController::get()->setOverlayColor(QColor(16,0,0,128));
}

void OmegaRpgMainWindow::nightModeGreen()
{
  NightModeController::get()->setOverlayColor(QColor(0,16,0,128));
}

void OmegaRpgMainWindow::nightModeBlue()
{
  NightModeController::get()->setOverlayColor(QColor(0,0,16,128));
}

void OmegaRpgMainWindow::aboutQt()
{
  QMessageBox::aboutQt(this,"About Qt");
}

void OmegaRpgMainWindow::aboutOmegaRPG()
{
  QString text =
      QString("OmegaRPG %1 (protocol version %2)\n\n"
              "OmegaRPG is a free, open-source Virtual Game Table application inspired by OpenRPG.\n\n"
              "OmegaRPG is licensed under the GNU General Public License Version 3.\n\n"
              "Website: None yet")
              .arg(Global::applicationVersion().toString(),Global::protocolVersion().toString());
  QMessageBox::about(this,"About OmegaRPG",text);
}

void OmegaRpgMainWindow::chatAreaNotification()
{
  if(orpg::Settings::get()->notifyAlternateChannel)
  {
    playSound();
  }
}

void OmegaRpgMainWindow::settingsSaveFailed()
{
  chat->printErrorMessage("Could not save Settings.");
}

void OmegaRpgMainWindow::aliasSaveFailed()
{
  chat->printErrorMessage("Could not save Alias Library.");
}

void OmegaRpgMainWindow::toogleInitiativeMenu()
{
  setInitiativeMenu(!initiativeShown);
}

void OmegaRpgMainWindow::exportProperties()
{
  QString file = QFileDialog::getSaveFileName(this,"Export Room Properties",importExportDirectory,"Room Property File (*.rp.json)");
  if(!file.isEmpty())
  {
    if(!file.endsWith(".rp.json"))
      file += ".rp.json";
    importExportDirectory = file;
    if(!orpg::ClientCore::get()->exportRoomProperties(file))
    {
      QMessageBox::critical(this,"Export Error","Could not export properties; Could not open file.");
    }
  }
}

void OmegaRpgMainWindow::importProperties()
{
  QString file = QFileDialog::getOpenFileName(this,"Import Room Properties",importExportDirectory,"Room Property File (*.rp.json)");
  if(!file.isEmpty())
  {
    importExportDirectory = file;
    if(!orpg::ClientCore::get()->importRoomProperties(file))
    {
      QMessageBox::critical(this,"Import Error","Could not import properties; Could not open file or file is invalid.");
    }
  }
}

void OmegaRpgMainWindow::roleChanged(PlayerRole role)
{
  if(role<ROLE_GM)
  {
    menuRoomProperties->setDisabled(true);
  }
  else
  {
    menuRoomProperties->setDisabled(false);
  }
}

void OmegaRpgMainWindow::reseedRandom()
{
  Dice::reseed();
}

void OmegaRpgMainWindow::setAliasSheetVisible(bool value)
{
  if(value)
  {
    QList<int> sizes = {playerList->width(),chat->width()};
    playerListSplitter->insertWidget(0,playerList);
    playerListSplitter->setSizes(sizes);
  }
  else
  {
    QList<int> sizes = {playerList->width(),chat->width()};
    bottomSplitter->insertWidget(0,playerList);
    bottomSplitter->setSizes(sizes);
  }
  playerList->setAliasSheetVisible(value);
}
