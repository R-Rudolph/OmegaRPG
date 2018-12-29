#include "player.h"

Player::Player(const QString& id, const QString& name, const QString& status)
{
  this->id = id;
  this->name = name;
  this->status = status;
}

Player::Player()
{

}
