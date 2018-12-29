#ifndef OMEGARPGSETTINGS_H
#define OMEGARPGSETTINGS_H

#include <QObject>
#include <QFile>
#include <QSaveFile>
#include <QJsonDocument>
#include <QJsonParseError>

#include <QtMath>
#include <QFont>
#include <QColor>
#include <QDir>
#include "core/client/alias/alias.h"
#include "core/multi/util/global.h"
#include "core/client/util/distancemeasure.h"

namespace orpg
{
  class Settings : public QObject
  {
    Q_OBJECT
  public:
    enum LoadError
    {
      LOAD_OK,
      LOAD_EXISTS_ERROR,
      LOAD_OPEN_ERROR,
      LOAD_JSON_ERROR
    };
  private:
    QStringList staticMetaServers;
    QStringList synchedMetaServers;
    QStringList customMetaServers;
    QJsonObject toJson() const;
    static Settings* settings;

    LoadError lastLoadError;
    QJsonParseError lastParseError;
  public:
    static Settings* get();

    Settings(QObject* parent=0); //loads standard settings
    Settings(const QString& filename, QObject* parent=0); //loads settings from file

    QString filename;

    Alias player;

    DistanceMeasure distanceMeasure;
    QFont font;
    QString typingText;
    QString typingStoppedText;
    QString idleText;
    int typingTime;
    int typingStoppedTime;
    int heartbeatTime;
    QString soundFilepath;
    qreal volume;
    qreal volumeBackup;
    bool notifyAlternateChannel;
    bool aliasPerChannel;
    bool nightModeTextEdit;
    bool logEnabled;
    QString loglocation;
    bool openRpgLogEnabled;
    QString openRpgLogLocation;
    QVector<QString> macro;
    int numServerHistory;
    int imageResolutionLimit;

    bool showPreviousMiniaturePosLine;

    void setTimings(int typing, int stoppedTyping, int heartbeat);
    void setStatusTexts(const QString& typing, const QString& stoppedTyping, const QString& idle);
    void setPlayer(const QString& name, const QColor& color);
    void setVolume(qreal volume);
    void setVolumeTemporary(qreal volume);
    void restoreVolume();
    qreal getCorrectedVolume();
    void setMetaServerUrl(const QString& url);
    QStringList getMetaServerUrls() const;
    QString getCustomMetaServersString() const;
    QString getLogLocation() const;
    QDir getLogDir() const;
    QString getOpenRpgLogLocation() const;
    void setServerHistoryNum(int num);
    void setFont(const QFont& font);
    QString getSoundfilePath() const;
    void setSoundfilePath(const QString &value);

    QString getLoadErrorString();

    LoadError getLoadError() const;

    qint64 getCacheSize() const;

  signals:
    void aliasChanged();
    void error(QString);
    void timingsChanged();
    void statusMessagesChanged();
    void playerChanged();
    void volumeChanged(qreal);
    void correctedVolumeChanged(qreal);
    void fontChanged(const QFont& font);
    void soundfilePathChanged(const QString& path);
    void nightModeWorkaroundChanged(bool);
    void saveFailed();
  public slots:
    void notifyAliasChange();
    void fontSizeIncrease();
    void fontSizeDecrease();
    void setFilename(const QString& filename);
    bool load(const QString& filename);
    bool save(const QString& filename);
    bool save();
    void setSettings(const orpg::Settings& settings);
    void setNightModeWorkaround(bool value);
  };

}

#endif // OMEGARPGSETTINGS_H
