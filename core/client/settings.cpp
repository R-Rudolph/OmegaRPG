#include "settings.h"
#include "objects/nightmode.h"
#include <QDebug>
namespace orpg
{
  Settings* Settings::settings = nullptr;

  QJsonObject Settings::toJson() const
  {
    QJsonObject json;
    json.insert("player",player.toJson());
    json.insert("typingText",typingText);
    json.insert("typingStoppedText",typingStoppedText);
    json.insert("idleText",idleText);
    json.insert("typingTime",typingTime);
    json.insert("typingStoppedTime",typingStoppedTime);
    json.insert("heartbeatTime",heartbeatTime);
    json.insert("volume",volumeBackup);
    json.insert("metaServer",customMetaServers.join(" "));
    json.insert("logLocation",loglocation);
    json.insert("font",font.toString());
    json.insert("imageResolutionLimit",imageResolutionLimit);
    json.insert("logEnabled",logEnabled);
    json.insert("nightModeTextEdit",nightModeTextEdit);
    json.insert("openRpgLogEnabled",openRpgLogEnabled);
    json.insert("openRpgLogLocation",openRpgLogLocation);
    json.insert("soundFilepath",soundFilepath);
    json.insert("notifyAlternateChannel",notifyAlternateChannel);
    json.insert("aliasPerChannel",aliasPerChannel);
    json.insert("showPreviousMiniaturePosLine",showPreviousMiniaturePosLine);
    json.insert("distanceMeasure",distanceMeasure.toInt());
    for(int i=0;i<13;i++)
      json.insert("f"+QString::number(i+1),macro[i]);
    json.insert("network_proxy",networkProxyData());
    qDebug() << "toJson" << networkProxyData();
    return json;
  }

  Settings::Settings(QObject* parent) : QObject(parent)
  {
    lastLoadError = LOAD_OK;
    lastParseError.error = QJsonParseError::NoError;
    staticMetaServers = Global::getStaticMetaServerUrls();
    font.setFamily("serif");
    font.setPixelSize(15);
    font.setStyleHint(QFont::Serif);
    player = Alias();
    typingText = "Typing";
    typingTime = 3000;
    typingStoppedText = "Stopped Typing";
    typingStoppedTime = 9000;
    idleText = "Idle";
    heartbeatTime = 10000;
    volumeBackup = 50;
    volume = 50;
    numServerHistory = 50;
    imageResolutionLimit = 0;
    logEnabled = true;
    openRpgLogEnabled = false;
    nightModeTextEdit = true;
    notifyAlternateChannel = true;
    aliasPerChannel = true;
    showPreviousMiniaturePosLine = true;
    macro.resize(13);
    for(int i=0;i<12;i++)
      macro[i] = "I found the F"+QString::number(i+1)+" macro key!";
    macro[12] = "I found the F13 macro key!... wait, what?";
  }

  Settings::Settings(const QString &filename, QObject *parent) : Settings(parent)
  {
    staticMetaServers = Global::getStaticMetaServerUrls();
    load(filename);
  }
  
  QJsonObject Settings::networkProxyData() const
  {
      return networkProxyData_;
  }
  
  void Settings::setNetworkProxyData(const QJsonObject& networkProxyData)
  {
      qDebug() << "setting..." << networkProxyData;
      networkProxyData_ = networkProxyData;
  }

  QString Settings::getSoundfilePath() const
  {
    if(soundFilepath.isEmpty())
    {
        return "qrc:sounds/quack.mp3";
    }
    else
    {
        return "file:"+soundFilepath;
    }
  }
  
  void Settings::setSoundfilePath(const QString &value)
  {
    if(soundFilepath!=value)
    {
      soundFilepath = value;
      emit soundfilePathChanged(getSoundfilePath());
    }
  }

  QString Settings::getLoadErrorString()
  {
    switch(lastLoadError)
    {
      case LOAD_OPEN_ERROR:
        return QString("Could not read settings file. Permission denied.");
      case LOAD_JSON_ERROR:
        return QString("Json Error for settings file: "+lastParseError.errorString());
      case LOAD_EXISTS_ERROR:
        return QString("Settings file not found. This is normal if you run OmegaRPG for the first time.");
      default:
        return QString();
    }
  }

  Settings::LoadError Settings::getLoadError() const
  {
    return lastLoadError;
  }

  qint64 Settings::getCacheSize() const
  {
    return 104857600;
  }
  
  void Settings::setTimings(int typing, int stoppedTyping, int heartbeat)
  {
      if(typing>=0)
          typingTime = typing;
      if(stoppedTyping>=0)
          typingStoppedTime = stoppedTyping;
    if(heartbeat>=0)
      heartbeatTime = heartbeat;
    emit timingsChanged();
  }

  void Settings::setStatusTexts(const QString &typing, const QString &stoppedTyping, const QString &idle)
  {
    typingText = typing;
    typingStoppedText = stoppedTyping;
    idleText = idle;
    emit statusMessagesChanged();
  }

  void Settings::setPlayer(const QString &name, const QColor &color)
  {
    player = Alias(name,color.name());
    emit playerChanged();
  }

  void Settings::setVolume(qreal volume)
  {
    this->volumeBackup = volume;
    if(this->volume!=volume)
    {
      this->volume = volume;
      emit volumeChanged(volume);
      emit correctedVolumeChanged(getCorrectedVolume());
    }
  }

  void Settings::setVolumeTemporary(qreal volume)
  {
    if(this->volume!=volume)
    {
      this->volume = volume;
      emit volumeChanged(volume);
      emit correctedVolumeChanged(getCorrectedVolume());
    }
  }

  void Settings::restoreVolume()
  {
    setVolume(volumeBackup);
  }

  void Settings::setMetaServerUrl(const QString &url)
  {
    if(url.isEmpty())
      customMetaServers.clear();
    else
      customMetaServers = url.split(" ");
  }

  QStringList Settings::getMetaServerUrls() const
  {
    QStringList list = staticMetaServers;
    foreach(const QString& server, synchedMetaServers)
    {
      if(!list.contains(server))
        list.append(server);
    }
    foreach(const QString& server, customMetaServers)
    {
      if(!list.contains(server))
        list.append(server);
    }
    return list;
  }

  QString Settings::getCustomMetaServersString() const
  {
    return customMetaServers.join(" ");
  }

  QString Settings::getLogLocation() const
  {
    if(loglocation.isEmpty())
      return Global::logFolder();
    return loglocation;
  }

  QDir Settings::getLogDir() const
  {
    return QDir(getLogLocation());
  }

  QString Settings::getOpenRpgLogLocation() const
  {
    if(openRpgLogLocation.isEmpty())
      return Global::openRpgLogFolder();
    return openRpgLogLocation;
  }

  qreal Settings::getCorrectedVolume()
  {
    return qPow(2,volume/100)-1;
  }

  void Settings::setServerHistoryNum(int num)
  {
    numServerHistory = num;
  }

  void Settings::setFont(const QFont &font)
  {
    this->font = font;
    emit fontChanged(this->font);
  }

  void Settings::notifyAliasChange()
  {
    emit aliasChanged();
    if(this==Settings::settings)
    {
      save(Global::settingsFilepath());
    }
  }

  void Settings::fontSizeIncrease()
  {
    font.setPixelSize(std::max(font.pixelSize()+1,1));
    emit fontChanged(font);
  }

  void Settings::fontSizeDecrease()
  {
    font.setPixelSize(std::max(font.pixelSize()-1,1));
    emit fontChanged(font);
  }

  Settings *Settings::get()
  {
    if(settings==nullptr)
    {
      settings = new Settings(Global::settingsFilepath());
      connect(settings,&orpg::Settings::nightModeWorkaroundChanged,NightModeController::get(),&NightModeController::setNightModeWorkaround);
    }
    return settings;
  }

  void Settings::setFilename(const QString &filename)
  {
    this->filename = filename;
  }

  void setIfString(const QJsonValue& json ,QString& dst)
  {
    if(json.isString())
      dst = json.toString();
  }

  void setIfInt(const QJsonValue& json ,int& dst)
  {
    int a = json.toInt(-1);
    if(a > 0)
      dst = a;
  }

  bool Settings::load(const QString &filename)
  {
    this->filename = filename;
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
      if(file.exists())
        lastLoadError = LOAD_OPEN_ERROR;
      else
        lastLoadError = LOAD_EXISTS_ERROR;
      return false;
    }
    QByteArray data = file.readAll();
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data,&err);
    file.close();
    if(err.error != QJsonParseError::NoError)
    {
      lastLoadError = LOAD_JSON_ERROR;
      lastParseError = err;
      return false;
    }

    QString metaServer;
    QJsonObject arr = doc.object();
    openRpgLogLocation = arr["openRpgLogLocation"].toString("");
    openRpgLogEnabled = arr["openRpgLogEnabled"].toBool(false);
    logEnabled = arr["logEnabled"].toBool(true);
    nightModeTextEdit = arr["nightModeTextEdit"].toBool(true);
    player = Alias(arr["player"].toObject());
    setIfString(arr["typingText"],typingText);
    setIfString(arr["typingStoppedText"],typingStoppedText);
    setIfString(arr["idleText"],idleText);
    setIfString(arr["metaServer"],metaServer);
    setMetaServerUrl(metaServer);
    setIfString(arr["logLocation"],loglocation);
    setIfInt(arr["typingTime"],typingTime);
    setIfInt(arr["typingStoppedTime"],typingStoppedTime);
    setIfInt(arr["heartbeatTime"],heartbeatTime);
    setVolume(arr["volume"].toDouble(50));
    notifyAlternateChannel = arr["notifyAlternateChannel"].toBool(true);
    aliasPerChannel = arr["aliasPerChannel"].toBool(true);
    soundFilepath = arr["soundFilepath"].toString();
    showPreviousMiniaturePosLine = arr["showPreviousMiniaturePosLine"].toBool(showPreviousMiniaturePosLine);
    distanceMeasure = DistanceMeasure::fromInt(arr["distanceMeasure"].toInt(0));
    QFont tempFont;
    if(tempFont.fromString(arr["font"].toString()))
    {
      if(!tempFont.family().isEmpty())
        this->font = tempFont;
    }
    for(int i=0;i<13;i++)
      setIfString(arr["f"+QString::number(i+1)],macro[i]);
    imageResolutionLimit = arr["imageResolutionLimit"].toInt();
    networkProxyData_ = arr["network_proxy"].toObject();

    lastLoadError = LOAD_OK;
    return true;
  }

  bool Settings::save(const QString &filename)
  {
    QSaveFile file(filename);
    if(filename.isEmpty())
      return false;
    if(!file.open(QIODevice::WriteOnly))
    {
      emit saveFailed();
      return false;
    }
    QJsonObject arr = toJson();
    QJsonDocument doc(arr);
    QByteArray data = doc.toJson();
    file.write(data);
    file.commit();
    return true;
  }

  bool Settings::save()
  {
    return save(this->filename);
  }

  void Settings::setSettings(const orpg::Settings& settings)
  {
    player = settings.player;

    typingText = settings.typingText;
    typingStoppedText = settings.typingStoppedText;
    idleText = settings.idleText;
    if(settings.typingTime > 0)
      typingTime = settings.typingTime;
    if(settings.typingStoppedTime > 0)
      typingStoppedTime = settings.typingStoppedTime;
    if(settings.heartbeatTime > 0)
      heartbeatTime = settings.heartbeatTime;
    customMetaServers = settings.customMetaServers;
    staticMetaServers = settings.staticMetaServers;
    synchedMetaServers = settings.synchedMetaServers;
    loglocation = settings.loglocation;
    imageResolutionLimit = settings.imageResolutionLimit;
    showPreviousMiniaturePosLine = settings.showPreviousMiniaturePosLine;
    distanceMeasure = settings.distanceMeasure;
    save();
  }

  void Settings::setNightModeWorkaround(bool value)
  {
    if(nightModeTextEdit!=value)
    {
      nightModeTextEdit = value;
      emit nightModeWorkaroundChanged(value);
    }
  }

}
