#ifndef MAPSETTINGSVARIABLE_H
#define MAPSETTINGSVARIABLE_H

#include "roomvariable.h"

class MapSettingsVariable : public RoomVariable
{
public:
  static const QString variableName;
private:
  int sizeX;
  int sizeY;
  bool hideGrid;
  bool lockBackground;
  QString gridColor;
  QString bgColor;
public:
  MapSettingsVariable();
  MapSettingsVariable(const QJsonValue& data);
  bool isValid() const;
  QJsonValue data() const;
  int getSizeX() const;
  void setSizeX(int value);
  int getSizeY() const;
  void setSizeY(int value);
  bool getHideGrid() const;
  void setHideGrid(bool value);
  QString getGridColor() const;
  void setGridColor(const QString &value);
  QString getBgColor() const;
  void setBgColor(const QString &value);
  bool getLockBackground() const;
  void setLockBackground(bool value);
  virtual const QString& name() const;
};

#endif // MAPSETTINGSVARIABLE_H
