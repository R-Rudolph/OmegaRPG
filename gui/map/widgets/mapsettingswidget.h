#ifndef MAPSETTINGSWIDGET_H
#define MAPSETTINGSWIDGET_H

#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QSize>
#include <QFileDialog>
#include <QMessageBox>
#include <QCheckBox>
#include "core/multi/util/global.h"
#include "widgets/colorselectbutton.h"

class MapSettingsVariable;

class MapSettingsWidget : public QScrollArea
{
  Q_OBJECT
  int sizeX, sizeY;
  QColor gridColor, bgColor;
  bool showGrid, lockBackground;
public:
  explicit MapSettingsWidget(QWidget *parent = 0);
  //layout
  QWidget* wrapperWidget;
  QGridLayout* layout;
  //map size stuff
  QLabel* sizeLabel;
  QLabel* xLabel;
  QSpinBox* xSizeBox;
  QSpinBox* ySizeBox;
  //color stuff
  QLabel* backgroundColorLabel;
  QLabel* gridColorLabel;
  ColorSelectButton* backgroundColorButton;
  ColorSelectButton* gridColorButton;
  //additional
  QLabel* showGridLabel;
  QCheckBox* showGridBox;
  QLabel* lockBackgroundLabel;
  QCheckBox* lockBackgroundBox;
  //buttons
  QPushButton* applyButton;
  QPushButton* clearButton;
  //functions
  int getWantedWidth();
  QSize sizeHint();
  void resetValues();
private slots:
  void publishSettings(bool);
  void clearMapSlot(bool);
  void loadMapSettings();
  void mapSettingsChanged(const MapSettingsVariable& settings);
};

#endif // MAPSETTINGSWIDGET_H
