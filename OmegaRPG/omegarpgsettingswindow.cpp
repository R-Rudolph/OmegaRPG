#include "omegarpgsettingswindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCloseEvent>
#include <QLineEdit>
#include <QLabel>
#include <QColorDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QSlider>
#include <QFontDialog>
#include <QFontComboBox>
#include "core/client/settings.h"
#include "core/multi/util/global.h"
#include "widgets/colorselectbutton.h"
#include "widgets/folderselectwidget.h"
#include "widgets/alias/aliaseditwidget.h"
#include <QTabFormWidget>

void OmegaRpgSettingsWindow::confirmSettings()
{
  orpg::Settings::get()->player = alias;
  orpg::Settings::get()->setStatusTexts(typingStatus->text(),typingStoppedStatus->text(),idleStatus->text());
  orpg::Settings::get()->setTimings(typingTime->value(),typingStoppedTime->value(),heartbeatTime->value());
  orpg::Settings::get()->setVolume(volumeSlider->value());
  orpg::Settings::get()->setServerHistoryNum(serverHistoryBox->value());
  orpg::Settings::get()->setMetaServerUrl(metaServerEdit->text());
  orpg::Settings::get()->logEnabled = logEnabledCheckbox->isChecked();
  orpg::Settings::get()->loglocation = logLocationEdit->text();
  orpg::Settings::get()->openRpgLogEnabled = openRpgLogEnabledCheckbox->isChecked();
  orpg::Settings::get()->setNightModeWorkaround(nightModeTextArrowCheckBox->isChecked());
  orpg::Settings::get()->openRpgLogLocation = openRpgLogLocationEdit->text();
  orpg::Settings::get()->setFont(font);
  orpg::Settings::get()->imageResolutionLimit = imageResolutionBox->value();
  for(int i=0;i<macroEdits.size();i++)
    orpg::Settings::get()->macro[i] = macroEdits[i]->text();
  orpg::Settings::get()->setSoundfilePath(audioFileSelect->text());
  orpg::Settings::get()->aliasPerChannel = aliasPerChannelBox->checkState()==Qt::Checked;
  orpg::Settings::get()->notifyAlternateChannel = notifyAlternateChannelBox->checkState()==Qt::Checked;
  orpg::Settings::get()->showPreviousMiniaturePosLine = showPreviousMiniaturePosLineBox->isChecked();
  orpg::Settings::get()->distanceMeasure = DistanceMeasure::fromInt(distanceMeasureBox->currentIndex());

  networkProxySelection_->save();
  orpg::Settings::get()->setNetworkProxyData(networkProxySelection_->getSettings().toJson());
  networkProxySelection_->getSettings().setApplicationProxy();

  orpg::Settings::get()->notifyAliasChange();
}

OmegaRpgSettingsWindow::OmegaRpgSettingsWindow(QWidget *parent) : NightModeOverlay(parent,Qt::Window)
{
  setWidget(new QWidget(this));
  suppressSound = false;
  this->setWindowTitle("OmegaRPG Settings");
  this->resize(800,600);

  //set up objects
  mainDivide = new QVBoxLayout(this->getWidget());
  tabs = new QTabFormWidget(this,true);
  hBoxButtons = new QHBoxLayout();
  mainDivide->addWidget(tabs);
  mainDivide->addLayout(hBoxButtons);

  buttonOk = new QPushButton("Ok");
  buttonCancel = new QPushButton("Cancel");
  //data field objects
  //customize
  aliasEdit = new AliasEditWidget(this);
  {
    QSizePolicy policy = aliasEdit->sizePolicy();
    policy.setVerticalStretch(1);
    aliasEdit->setSizePolicy(policy);
  }
  typingStatus = new QLineEdit();
  typingStoppedStatus = new QLineEdit();
  idleStatus = new QLineEdit();
  //general
  notifyAlternateChannelBox = new QCheckBox("",this);
  notifyAlternateChannelBox->setToolTip("Play notify sound for messages in unselected channels.");
  aliasPerChannelBox = new QCheckBox("",this);
  aliasPerChannelBox->setToolTip("Remeber alias for each channel individually.");
  audioFileSelect = new FolderSelectWidget(this,true);
  audioFileSelect->setToolTip("Leave empty for default sound.");
  volumeSlider = new QSlider(Qt::Horizontal);
  volumeSlider->setMinimum(0);
  volumeSlider->setMaximum(100);
  logEnabledCheckbox = new QCheckBox(this);
  logLocationEdit = new FolderSelectWidget(this);
  logLocationEdit->setPlaceholderText(Global::logFolder());
  openRpgLogEnabledCheckbox = new QCheckBox(this);
  nightModeTextArrowCheckBox = new QCheckBox(this);
  nightModeTextArrowCheckBox->setToolTip("Make the mouse cursor for text fields during night mode an arrow.\n"
                                         "This is a workaround for barely visible cursors.");
  openRpgLogLocationEdit = new FolderSelectWidget(this);
  openRpgLogLocationEdit->setPlaceholderText(Global::openRpgLogFolder());
  fontSelection = new QFontComboBox(this);
  fontSizeBox = new QSpinBox(this);
  fontSizeBox->setMinimum(1);
  fontSizeBox->setMaximum(100);
  //map
  distanceMeasureBox = new QComboBox(this);
  foreach(DistanceMeasure dm, DistanceMeasure::getAll())
    distanceMeasureBox->addItem(dm.name());
  distanceMeasureBox->setToolTip("The measurement used for the measurement tools and miniaure moving.");

  showPreviousMiniaturePosLineBox = new QCheckBox(this);
  showPreviousMiniaturePosLineBox->setToolTip("If checked when hovering the mouse over a miniature, a line pointing to its last position will be displayed.");
  //system
  metaServerEdit = new QLineEdit(this);
  typingTime = new QSpinBox();
  typingStoppedTime = new QSpinBox();
  heartbeatTime = new QSpinBox();
  serverHistoryBox = new QSpinBox();
  typingTime->setMaximum(1000000);
  typingStoppedTime->setMaximum(1000000);
  heartbeatTime->setMaximum(1000000);
  //
  networkProxySelection_ = new qt_utils::QNetworkProxyWidget({QNetworkProxy::NoProxy,QNetworkProxy::DefaultProxy,QNetworkProxy::Socks5Proxy,QNetworkProxy::HttpCachingProxy},this);
  //performance
  imageResolutionBox = new QSpinBox();
  imageResolutionBox->setSuffix(" MPixel");
  imageResolutionBox->setSpecialValueText("Unlimited");
  imageResolutionBox->setToolTip("The maximum resolution of a miniature image (in megapixels) before it is rescaled. This reduces memory consumption for large images. 0 for unlimited.");
  imageResolutionBox->setMinimum(0);
  imageResolutionBox->setMaximum(2000);

  //buttons
  hBoxButtons->addWidget(buttonOk);
  hBoxButtons->addWidget(buttonCancel);

  //data fields
  //custom
  tabs->addWidget("Customize/alias",aliasEdit,LabelPolicy::None);
  tabs->addWidget("Customize/'Typing' Status",typingStatus,LabelPolicy::Show);
  tabs->addWidget("Customize/'Typing Stopped' Status",typingStoppedStatus,LabelPolicy::Show);
  tabs->addWidget("Customize/'Idle' Status",idleStatus,LabelPolicy::Show);
  //general
  tabs->addWidget("General/Notification Sound",audioFileSelect,LabelPolicy::Show);
  tabs->addWidget("General/Volume",volumeSlider,LabelPolicy::Show);
  tabs->addWidget("General/Alternate Channel Notify",notifyAlternateChannelBox,LabelPolicy::Show);
  tabs->addWidget("General/Alias Per Channel",aliasPerChannelBox,LabelPolicy::Show);
  tabs->addWidget("General/Nightmode Workaround",nightModeTextArrowCheckBox,LabelPolicy::Show);
  tabs->addWidget("General/Enable Logging",logEnabledCheckbox,LabelPolicy::Show);
  tabs->addWidget("General/Log Location",logLocationEdit,LabelPolicy::Show);
  tabs->addWidget("General/OpenRPG Logging",openRpgLogEnabledCheckbox,LabelPolicy::Show);
  tabs->addWidget("General/OpenRPG Log Location",openRpgLogLocationEdit,LabelPolicy::Show);
  tabs->addWidget("General/Font",fontSelection,LabelPolicy::Show);
  tabs->addWidget("General/Font Size",fontSizeBox,LabelPolicy::Show);
  //map
  tabs->addWidget("Map/Distance Measurement",distanceMeasureBox);
  tabs->addWidget("Map/Miniature Movement Line",showPreviousMiniaturePosLineBox,LabelPolicy::Show);
  //macros
  for(int i=0;i<orpg::Settings::get()->macro.size();i++)
  {
    macroEdits.push_back(new QLineEdit());
    tabs->addWidget("Macros/F"+QString::number(i+1)+"-Macro",macroEdits[i]);
  }
  //network proxy
  tabs->addWidget("Network Proxy/proxy",networkProxySelection_,LabelPolicy::None);
  //performance
  tabs->addWidget("Performance/Max. Image Size",imageResolutionBox,LabelPolicy::Show);
  //system
  tabs->addWidget("System/Additional Meta Server",metaServerEdit,LabelPolicy::Show);
  tabs->addWidget("System/Typing Time\\/ms",typingTime,LabelPolicy::Show);
  tabs->addWidget("System/Typing Stopped Time\\/ms",typingStoppedTime,LabelPolicy::Show);
  tabs->addWidget("System/Heartbeat Time\\/ms",heartbeatTime,LabelPolicy::Show);
  tabs->addWidget("System/Server History Entries",serverHistoryBox,LabelPolicy::Show);

  //connect signals
  connect(buttonCancel,&QAbstractButton::clicked,this,&QWidget::close);
  connect(buttonOk,&QAbstractButton::clicked,this,&OmegaRpgSettingsWindow::ok);
  connect(volumeSlider,&QAbstractSlider::valueChanged,this,&OmegaRpgSettingsWindow::volumeChanged);
  connect(fontSelection,&QFontComboBox::currentFontChanged,this,&OmegaRpgSettingsWindow::currentChatFontChanged);
  connect(fontSizeBox,qOverload<int>(&QSpinBox::valueChanged),this,&OmegaRpgSettingsWindow::fontSizeChanged);
  connect(orpg::Settings::get(),&orpg::Settings::fontChanged,this,&OmegaRpgSettingsWindow::setChatFont);
  connect(orpg::Settings::get(),&orpg::Settings::aliasChanged,this,&OmegaRpgSettingsWindow::reloadAlias);
}

bool OmegaRpgSettingsWindow::restoreGeometry(const QByteArray &geometry)
{
  bool result = QWidget::restoreGeometry(geometry);
  return result;
}

void OmegaRpgSettingsWindow::closeEvent(QCloseEvent *event)
{
  lastSize = QWidget::size();
  lastPosition = QWidget::pos();
  orpg::Settings::get()->restoreVolume();
  setSettings(orpg::Settings::get());
  event->accept();
}

void OmegaRpgSettingsWindow::show()
{
  QWidget::show();
  if(lastSize.isValid())
  {
    resize(QSize(0,0));
    move(lastPosition);
    resize(lastSize);
  }
}

void OmegaRpgSettingsWindow::setChatFont(const QFont &font)
{
  this->font = font;
  this->font.setBold(false);
  this->font.setItalic(false);
  this->font.setStrikeOut(false);
  this->font.setOverline(false);
  this->font.setUnderline(false);
  this->fontSizeBox->setValue(font.pixelSize());
  fontSelection->setCurrentFont(font);
}

void OmegaRpgSettingsWindow::reloadAlias()
{
  alias = orpg::Settings::get()->player;
  aliasEdit->setAlias(&alias);
}

void OmegaRpgSettingsWindow::ok(bool)
{
  confirmSettings();
  close();
}

void OmegaRpgSettingsWindow::setSettings(const orpg::Settings *settings)
{
  alias = settings->player;
  aliasEdit->setAlias(&alias);

  typingStatus->setText(settings->typingText);
  typingStoppedStatus->setText(settings->typingStoppedText);
  idleStatus->setText(settings->idleText);

  nightModeTextArrowCheckBox->setChecked(settings->nightModeTextEdit);
  logEnabledCheckbox->setChecked(settings->logEnabled);
  logLocationEdit->setText(settings->loglocation);
  openRpgLogEnabledCheckbox->setChecked(settings->openRpgLogEnabled);
  openRpgLogLocationEdit->setText(settings->openRpgLogLocation);
  metaServerEdit->setText(settings->getCustomMetaServersString());
  typingTime->setValue(settings->typingTime);
  typingStoppedTime->setValue(settings->typingStoppedTime);
  heartbeatTime->setValue(settings->heartbeatTime);
  setChatFont(settings->font);
  audioFileSelect->setText(settings->soundFilepath);
  aliasPerChannelBox->setChecked(settings->aliasPerChannel);
  notifyAlternateChannelBox->setChecked(settings->notifyAlternateChannel);
  showPreviousMiniaturePosLineBox->setChecked(settings->showPreviousMiniaturePosLine);
  distanceMeasureBox->setCurrentIndex(settings->distanceMeasure.toInt());

  suppressSound = true;
  volumeSlider->setValue(static_cast<int>(settings->volume));
  suppressSound = false;

  serverHistoryBox->setValue(settings->numServerHistory);

  //macros
  for(int i=0;i<macroEdits.size();i++)
  {
    macroEdits[i]->setText(settings->macro[i]);
  }

  //performance
  imageResolutionBox->setValue(settings->imageResolutionLimit);
  //network proxy
  networkProxySelection_->loadJson(settings->networkProxyData());
}

void OmegaRpgSettingsWindow::volumeChanged(int)
{
  orpg::Settings::get()->setVolumeTemporary(volumeSlider->value());
  if(!suppressSound)
  {
    emit produceSound();
  }
}

void OmegaRpgSettingsWindow::currentChatFontChanged(const QFont &font)
{
  this->font = font;
  this->font.setBold(false);
  this->font.setItalic(false);
  this->font.setStrikeOut(false);
  this->font.setOverline(false);
  this->font.setUnderline(false);
}

void OmegaRpgSettingsWindow::fontSizeChanged(int size)
{
  font.setPixelSize(size);
}
