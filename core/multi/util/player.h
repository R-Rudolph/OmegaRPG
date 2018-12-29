#ifndef PLAYER_H
#define PLAYER_H

#include <QString>


enum PlayerRole
{
  ROLE_NOINTRODUCTION,
  ROLE_LURKER,
  ROLE_PLAYER,
  ROLE_GM,
  ROLE_ADMIN,
  ROLE_INTERNAL,
  ROLE_NOTSET
};

class Player
{
public:
    QString id;
    QString name;
    QString status;
    PlayerRole role;
    Player(const QString& id, const QString& name, const QString& status);
    Player();
};

#endif // PLAYER_H
