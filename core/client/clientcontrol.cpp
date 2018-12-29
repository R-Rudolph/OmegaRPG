#include "clientcontrol.h"

namespace orpg
{

  void ClientControl::sendMessage(Message msg)
  {
    core->client->sendMessage(msg);
  }

  ClientControl::ClientControl(QObject *parent) : QObject(parent)
  {
    settings = Settings::get();
    core = ClientCore::get();
    core->setParent(this);
    core->setName(settings->player.getName());
    core->setStatus(settings->idleText);

    typingTimer = new QTimer(this);
    typingTimer->setSingleShot(true);
    typingStoppedTimer = new QTimer(this);
    typingStoppedTimer->setSingleShot(true);

    QObject::connect(typingTimer,&QTimer::timeout,this,&ClientControl::stoppedTyping);
    QObject::connect(typingStoppedTimer,&QTimer::timeout,this,&ClientControl::idle);

    QObject::connect(core,&ClientCore::errorMessage,this,&ClientControl::errorMessage);
    QObject::connect(core,&ClientCore::statusMessage,this,&ClientControl::statusMessage);
    QObject::connect(core,&ClientCore::sslErrors,this,&ClientControl::sslErrors);

    QObject::connect(core,&ClientCore::receivedChat,this,&ClientControl::receivedChat);
    QObject::connect(this,&ClientControl::publishSettings,settings,&Settings::setSettings);

    QObject::connect(core,&ClientCore::playersChanged,this,&ClientControl::playersChanged);
    QObject::connect(core,&ClientCore::roomsChanged,this,&ClientControl::roomsChanged);

    QObject::connect(core,&ClientCore::roleChanged,this,&ClientControl::roleChanged);
    QObject::connect(core,&ClientCore::idChanged,this,&ClientControl::idChanged);

    QObject::connect(settings,&Settings::playerChanged,this,&ClientControl::resendStatus);
    QObject::connect(settings,&Settings::statusMessagesChanged,this,&ClientControl::resendStatus);

    QObject::connect(core,&ClientCore::connectedSignal,this,&ClientControl::connected);
    QObject::connect(core,&ClientCore::disconnectedSignal,this,&ClientControl::disconnected);
  }

  const Settings *ClientControl::getSettings()
  {
    return settings;
  }

  void ClientControl::chat(QString text, QString to)
  {
    const Alias* alias = AliasLib::get()->getCurrentAlias();
    core->chat(alias->getName(),alias->getColor(),text,to);
  }

  void ClientControl::connect(QString address)
  {
    core->connect(address);
  }

  void ClientControl::typing()
  {
    core->player(settings->typingText,settings->player.getName());
    typingTimer->start(settings->typingTime);
  }

  void ClientControl::stoppedTyping()
  {
    core->player(settings->typingStoppedText,settings->player.getName());
    typingStoppedTimer->start(settings->typingStoppedTime);
  }

  void ClientControl::idle()
  {
    typingTimer->stop();
    typingStoppedTimer->stop();
    core->player(settings->idleText,settings->player.getName());
  }

  void ClientControl::disconnectFromHost()
  {
    core->disconnectFromHost();
  }

  void ClientControl::joinRoom(orpg::JoinRoomMessage room)
  {
    core->joinRoom(room);
  }

  void ClientControl::createRoom(QString name, QString pass, QString dmpass)
  {
    core->createRoom(name,pass,dmpass);
  }

  void ClientControl::assignStatus(quint32 id, PlayerRole status, QString dmpass)
  {
    core->assignStatus(id,status,dmpass);
  }

  void ClientControl::kick(quint32 id)
  {
    core->kick(id);
  }

  void ClientControl::announceCurrentState()
  {
    core->announceCurrentState();
  }

  void ClientControl::resendStatus()
  {
    if(typingTimer->remainingTime()>0)
      core->player(settings->typingText,settings->player.getName());
    else if(typingStoppedTimer->remainingTime()>0)
      core->player(settings->typingStoppedText,settings->player.getName());
    else
      core->player(settings->idleText,settings->player.getName());
  }

  void ClientControl::sslErrors(const QList<QSslError> &errors)
  {
    bool ok = true;
    foreach(QSslError error,errors)
    {
      if(error.certificate().isNull())
      {
        QMessageBox mb("OmegaRPG",
                               "SSL Error:\n"+error.errorString()+"\nConnect anyway?",
                               QMessageBox::Warning,
                               QMessageBox::Yes,
                               QMessageBox::No | QMessageBox::Escape | QMessageBox::Default,
                               QMessageBox::NoButton);
        if(mb.exec()==QMessageBox::Yes)
        {
          SslSettings::get()->addTemporaryException(error);
        }
        else
        {
          emit errorMessage(error.errorString());
          ok = false;
        }
      }
      else
      {
        CertificateDialog dialog(error.certificate(),error.errorString());
        switch(dialog.exec())
        {
          case CertificateDialog::YesPermanently:
            SslSettings::get()->addPermanentException(error);
            break;
          case CertificateDialog::YesTemporary:
            SslSettings::get()->addTemporaryException(error);
            break;
          case CertificateDialog::No:
            emit errorMessage(error.errorString());
            ok = false;
            break;
        }
      }
      if(!ok)
        break;
    }
  }

}
