#ifndef OMEGARPGMAINWINDOW_H
#define OMEGARPGMAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QTextBrowser>
#include <QTextEdit>
#include <QSplitter>
#include <QMessageBox>
#include <QMenuBar>
#include <QCloseEvent>
#include <QTreeWidget>
#include <QComboBox>

#include <QApplication>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QAudioDecoder>
#include <QSoundEffect>
#include <QSound>
#include <QSettings>
#include <QGraphicsColorizeEffect>

#include "widgets/chattextedit.h"



#include "widgets/alias/aliaswindow.h"
#include "core/client/clientcontrol.h"
#include "omegarpgsettingswindow.h"
#include "omegarpgconnectionwindow.h"
#include "gui/map/widgets/mapwidget.h"
#include "widgets/chatareawidget.h"
#include "widgets/playerlist.h"
#include "widgets/servercontrolwidget.h"
#include "dialogs/knowncertificatesdialog.h"
#include "widgets/logviewerwindow.h"
#include "widgets/playerlistareawidget.h"
#include "widgets/nightmodeoverlay.h"
#include "gui/client/plugins/pluginlibrarywidget.h"

class OmegaRpgMainWindow : public QMainWindow
{
    Q_OBJECT
    //main widget stuff
    QIcon icon;
    NightModeOverlay* mainWindowWidget;
    QVBoxLayout* layout;
    KnownCertificatesWindow* knownCertificateDialog;

    //content
    QSplitter* mainSplitter;
    QSplitter* playerListSplitter;
    QSplitter* bottomSplitter;
    QSplitter* playerAliasSplitter;
    MapWidget* map;
    ChatAreaWidget* chat;
    PlayerListAreaWidget* playerList;

    //menu bar stuff
    QMenu* menuOmegaRpg;
    QAction* actionSettingsMenu;
    QAction* actionConnectionMenu;
    QMenu* menuWindows;
    QAction* actionAliasMenu;
    QAction* initiativeAction;
    QMenu* menuRoomProperties;
    QAction* exportRoomPropertieseAction;
    QAction* importRoomPropertieseAction;
    QString importExportDirectory;
    QMenu* menuHelp;

    //control
    orpg::ClientControl* control;

    //multimedia
    QMediaPlayer* sfx;
    bool suppressSound;

    void setInitiativeMenu(bool value);
    bool initiativeShown;

    void restoreWindow();
    void saveWindow();
public:
    //other windows
    OmegaRpgConnectionWindow* connectionWindow;
    AliasWindow* aliasWindow;
    OmegaRpgSettingsWindow* settingsWindow;
    ServerControlWidget* serverWindow;
    LogViewerWindow* logWindow;
    PluginLibraryWidget* pluginWindow_;

    explicit OmegaRpgMainWindow();
protected:
    void closeEvent(QCloseEvent *event);
signals:
public slots:
    void setSoundVolume(qreal volume);
    void playSound();
    void setSound(const QString& path);
    void notify();
    void updateSettingsWindow();
private slots:
    void nightModeGrey();
    void nightModeRed();
    void nightModeGreen();
    void nightModeBlue();
    void aboutQt();
    void aboutOmegaRPG();
    void chatAreaNotification();
    void settingsSaveFailed();
    void aliasSaveFailed();
    void toogleInitiativeMenu();
    void exportProperties();
    void importProperties();
    void roleChanged(PlayerRole role);
    void reseedRandom();
    void setAliasSheetVisible(bool value);
};

#endif // OMEGARPGMAINWINDOW_H
