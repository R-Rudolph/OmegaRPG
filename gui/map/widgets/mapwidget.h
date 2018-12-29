#ifndef OMEGARPGMAP_H
#define OMEGARPGMAP_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QSplitter>

#include "../objects/mapscene.h"
#include "mapview.h"
#include "core/multi/util/player.h"

#include "miniaturemenuwidget.h"
#include "mapsettingswidget.h"
#include "widgets/colorselectbutton.h"

class InitiativeWidget;

class MapWidget : public QWidget
{
  Q_OBJECT
  QVBoxLayout* mainLayout;
  QHBoxLayout* buttonLayout;
  MapView* view;

  QPushButton* measureButton;
  QPushButton* cursorButton;
  QPushButton* pencilButton;
  QPushButton* eraserButton;
  QPushButton* lineButton;
  QPushButton* rectangleButton;
  QPushButton* textButton;
  ColorSelectButton* colorButton;
  QPushButton* fogButton;
  QPushButton* clearfogButton;
  QPushButton* miniatureButton;
  QPushButton* mapSettingsButton;

  QSpinBox* brushSizeBox;
  QComboBox* snappingBox;

  MiniatureMenuWidget* miniatureMenu;
  MapSettingsWidget* mapSettingsWidget;
  QSplitter* mapMenuSplitter;
  InitiativeWidget* initiativeWidget;

  void resetToolButtons();
  PlayerRole cStatus;

  void setDisabledDrawingButtons(bool value);
  void setDisabledFogButtons(bool value);
public:
  MapScene* scene;
  explicit MapWidget(QWidget *parent = 0);
signals:
public slots:
  void measureButtonSlot();
  void cursorButtonSlot();
  void pencilButtonSlot();
  void eraserButtonSlot();
  void lineButtonSlot();
  void rectangleButtonSlot();
  void textButtonSlot();
  void fogButtonSlot();
  void clearfogButtonSlot();
  void miniatureButtonSlot();
  void mapSettingsButtonSlot();
  void snappingSelected(int index);
  void setClientStatus(PlayerRole status);
  void showInitiative(bool value);
};

#endif // OMEGARPGMAP_H
