#include "mapsettingsvariable.h"
#include<QJsonArray>
const QString MapSettingsVariable::variableName = "mapsettings";

int MapSettingsVariable::getSizeX() const
{
  return sizeX;
}

void MapSettingsVariable::setSizeX(int value)
{
  sizeX = value;
}

int MapSettingsVariable::getSizeY() const
{
  return sizeY;
}

void MapSettingsVariable::setSizeY(int value)
{
  sizeY = value;
}

bool MapSettingsVariable::getHideGrid() const
{
  return hideGrid;
}

void MapSettingsVariable::setHideGrid(bool value)
{
  hideGrid = value;
}

QString MapSettingsVariable::getGridColor() const
{
  return gridColor;
}

void MapSettingsVariable::setGridColor(const QString &value)
{
  gridColor = value;
}

QString MapSettingsVariable::getBgColor() const
{
  return bgColor;
}

void MapSettingsVariable::setBgColor(const QString &value)
{
  bgColor = value;
}

bool MapSettingsVariable::getLockBackground() const
{
  return lockBackground;
}

void MapSettingsVariable::setLockBackground(bool value)
{
  lockBackground = value;
}

const QString&MapSettingsVariable::name() const
{
  return variableName;
}

MapSettingsVariable::MapSettingsVariable()
{
  sizeX = 50;
  sizeY = 50;
  hideGrid = false;
  lockBackground = false;
  gridColor = "#808080";
  bgColor = "#FFFFFF";
}

MapSettingsVariable::MapSettingsVariable(const QJsonValue &data)
  :MapSettingsVariable()
{
  QJsonArray dataArray = data.toArray();
  sizeX = dataArray[0].toInt(-1);
  sizeY = dataArray[1].toInt(-1);
  hideGrid = dataArray[2].toBool(hideGrid);
  gridColor = dataArray[3].toString(gridColor);
  bgColor = dataArray[4].toString(bgColor);
  lockBackground = dataArray[5].toBool(false);
}

bool MapSettingsVariable::isValid() const
{
  return (sizeX >= 0) && (sizeY >= 0);
}

QJsonValue MapSettingsVariable::data() const
{
  QJsonArray data;
  data.append(sizeX);
  data.append(sizeY);
  data.append(hideGrid);
  data.append(gridColor);
  data.append(bgColor);
  data.append(lockBackground);
  return data;
}
