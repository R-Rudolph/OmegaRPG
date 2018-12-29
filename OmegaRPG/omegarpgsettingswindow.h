#ifndef OMEGARPGSETTINGSWINDOW_H
#define OMEGARPGSETTINGSWINDOW_H

#include <QWidget>
#include <QComboBox>
#include "core/client/alias/alias.h"
#include "widgets/nightmodeoverlay.h"

class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class AliasEditWidget;
class QLineEdit;
class QSlider;
class QCheckBox;
class QFontComboBox;
class QSpinBox;

class FolderSelectWidget;
class QTabFormWidget;

namespace orpg
{
  class Settings;
}

class OmegaRpgSettingsWindow : public NightModeOverlay
{
  Q_OBJECT
  QSize lastSize;
  QPoint lastPosition;
  QVBoxLayout* mainDivide;
  QTabFormWidget* tabs;
  QHBoxLayout* hBoxButtons;
  QPushButton* buttonOk;
  QPushButton* buttonCancel;

  QFont font;

  //data fields
  bool suppressSound;
  Alias alias;

  //customize
  AliasEditWidget* aliasEdit;
  QLineEdit* typingStatus;
  QLineEdit* typingStoppedStatus;
  QLineEdit* idleStatus;
  //general
  FolderSelectWidget* audioFileSelect;
  QSlider* volumeSlider;
  QCheckBox* notifyAlternateChannelBox;
  QCheckBox* aliasPerChannelBox;
  QCheckBox* logEnabledCheckbox;
  QCheckBox* nightModeTextArrowCheckBox;
  FolderSelectWidget* logLocationEdit;
  QCheckBox* openRpgLogEnabledCheckbox;
  FolderSelectWidget* openRpgLogLocationEdit;
  QFontComboBox* fontSelection;
  QSpinBox* fontSizeBox;
  //map
  QComboBox* distanceMeasureBox;
  QCheckBox* showPreviousMiniaturePosLineBox;
  //system
  QLineEdit* metaServerEdit;
  QSpinBox* typingTime;
  QSpinBox* typingStoppedTime;
  QSpinBox* heartbeatTime;
  QSpinBox* serverHistoryBox;
  //macros
  QVector<QLineEdit*> macroEdits;
  //performance
  QSpinBox* imageResolutionBox;

  void confirmSettings();
public:
  explicit OmegaRpgSettingsWindow(QWidget *parent = 0);
  bool restoreGeometry(const QByteArray &geometry);
protected:
  void closeEvent(QCloseEvent *event);
signals:
  void demandSettings();
  void publishSettings(const orpg::Settings&);
  void produceSound();
public slots:
  void show();
  void setChatFont(const QFont& font);
  void reloadAlias();
  void ok(bool);
  void setSettings(const orpg::Settings* settings);
private slots:
  void volumeChanged(int value);
  void currentChatFontChanged(const QFont& font);
  void fontSizeChanged(int size);
};

#endif // OMEGARPGSETTINGSWINDOW_H
