#include "mapwidget.h"
#include "objects/nightmode.h"
#include <QOpenGLWidget>
#include "widgets/initiativewidget.h"

void MapWidget::resetToolButtons()
{
  measureButton->setDown(false);
  cursorButton->setDown(false);
  pencilButton->setDown(false);
  eraserButton->setDown(false);
  lineButton->setDown(false);
  rectangleButton->setDown(false);
  textButton->setDown(false);
  fogButton->setDown(false);
  clearfogButton->setDown(false);
  miniatureMenu->hide();
  miniatureButton->setDown(false);
}

void MapWidget::setDisabledDrawingButtons(bool value)
{
  colorButton->setDisabled(value);
  cursorButton->setDisabled(value);
  pencilButton->setDisabled(value);
  eraserButton->setDisabled(value);
  lineButton->setDisabled(value);
  rectangleButton->setDisabled(value);
  textButton->setDisabled(value);
  snappingBox->setDisabled(value);
  brushSizeBox->setDisabled(value);
  miniatureButton->setDisabled(value);
}

void MapWidget::setDisabledFogButtons(bool value)
{
  fogButton->setDisabled(value);
  clearfogButton->setDisabled(value);
  mapSettingsButton->setDisabled(value);
}

MapWidget::MapWidget(QWidget *parent) : QWidget(parent)
{
  mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(0,0,0,0);

  //button layout
  buttonLayout = new QHBoxLayout();
  buttonLayout->setContentsMargins(0,0,0,0);
  scene = new MapScene(this);
  view = new MapView(scene,this);

  //submenus
  initiativeWidget = new InitiativeWidget(this);
  initiativeWidget->setMinimumWidth(200);
  mapMenuSplitter = new QSplitter(Qt::Horizontal);
  miniatureMenu = new MiniatureMenuWidget();
  mapSettingsWidget = new MapSettingsWidget();
  mapMenuSplitter->setStretchFactor(0,5);

  //buttons
  measureButton = new QPushButton("",this);
  measureButton->setIcon(QIcon(":icons/circlecompass.png"));
  measureButton->setIconSize(QSize(30,30));
  measureButton->setToolTip("Measurement");
  measureButton->setFocusPolicy(Qt::NoFocus);
  connect(measureButton,&QPushButton::clicked,this,&MapWidget::measureButtonSlot);

  cursorButton = new QPushButton("");
  cursorButton->setIcon(QIcon(":icons/cursor.png"));
  cursorButton->setIconSize(QSize(30,30));
  cursorButton->setToolTip("Cursor");
  cursorButton->setFocusPolicy(Qt::NoFocus);
  connect(cursorButton,&QAbstractButton::clicked,this,&MapWidget::cursorButtonSlot);

  pencilButton = new QPushButton("");
  pencilButton->setIcon(QIcon(":icons/paintbrush.png"));
  pencilButton->setIconSize(QSize(30,30));
  pencilButton->setToolTip("Pencil");
  pencilButton->setFocusPolicy(Qt::NoFocus);
  connect(pencilButton,&QAbstractButton::clicked,this,&MapWidget::pencilButtonSlot);

  lineButton = new QPushButton("");
  lineButton->setIcon(QIcon(":icons/pencil.png"));
  lineButton->setIconSize(QSize(30,30));
  lineButton->setToolTip("Line");
  lineButton->setFocusPolicy(Qt::NoFocus);
  connect(lineButton,&QAbstractButton::clicked,this,&MapWidget::lineButtonSlot);

  rectangleButton = new QPushButton("");
  rectangleButton->setIcon(QIcon(":icons/rectangle_blue.png"));
  rectangleButton->setIconSize(QSize(30,30));
  rectangleButton->setToolTip("Rectangle");
  rectangleButton->setFocusPolicy(Qt::NoFocus);
  connect(rectangleButton,&QAbstractButton::clicked,this,&MapWidget::rectangleButtonSlot);

  textButton = new QPushButton("",this);
  textButton->setIcon(QIcon(":/icons/typography.png"));
  textButton->setIconSize(QSize(30,30));
  textButton->setToolTip("Text");
  textButton->setFocusPolicy(Qt::NoFocus);
  connect(textButton,&QPushButton::clicked,this,&MapWidget::textButtonSlot);

  eraserButton = new QPushButton("");
  eraserButton->setIcon(QIcon(":icons/water.png"));
  eraserButton->setIconSize(QSize(30,30));
  eraserButton->setToolTip("Eraser");
  eraserButton->setFocusPolicy(Qt::NoFocus);
  connect(eraserButton,&QAbstractButton::clicked,this,&MapWidget::eraserButtonSlot);

  colorButton = new ColorSelectButton(0,Qt::black);
  colorButton->setToolTip("Select Brush Color");
  connect(colorButton,&ColorSelectButton::colorChanged,scene,&MapScene::setBrushColor);

  clearfogButton = new QPushButton("");
  clearfogButton->setIcon(QIcon(":icons/bright.png"));
  clearfogButton->setIconSize(QSize(30,30));
  clearfogButton->setToolTip("Clear Fog");
  clearfogButton->setFocusPolicy(Qt::NoFocus);
  connect(clearfogButton,&QAbstractButton::clicked,this,&MapWidget::clearfogButtonSlot);


  fogButton = new QPushButton("");
  fogButton->setIcon(QIcon(":icons/dark.png"));
  fogButton->setIconSize(QSize(30,30));
  fogButton->setToolTip("Fog of War");
  fogButton->setFocusPolicy(Qt::NoFocus);
  connect(fogButton,&QAbstractButton::clicked,this,&MapWidget::fogButtonSlot);


  miniatureButton = new QPushButton("");
  miniatureButton->setIcon(QIcon(":icons/miniature.png"));
  miniatureButton->setIconSize(QSize(30,30));
  miniatureButton->setToolTip("Miniatures");
  miniatureButton->setFocusPolicy(Qt::NoFocus);
  connect(miniatureButton,&QAbstractButton::clicked,this,&MapWidget::miniatureButtonSlot);


  mapSettingsButton = new QPushButton("");
  mapSettingsButton->setIcon(QIcon(":icons/tools.png"));
  mapSettingsButton->setIconSize(QSize(30,30));
  mapSettingsButton->setToolTip("Map Settings");
  mapSettingsButton->setFocusPolicy(Qt::NoFocus);
  connect(mapSettingsButton,&QAbstractButton::clicked,this,&MapWidget::mapSettingsButtonSlot);

  //Brush size select
  brushSizeBox = new QSpinBox();
  brushSizeBox->setToolTip("Brush Size");
  connect(brushSizeBox,qOverload<int>(&QSpinBox::valueChanged),scene,&MapScene::setBrushSize);
  brushSizeBox->setValue(3);
  brushSizeBox->setMinimumWidth(40);

  //snap to grid select
  snappingBox = new QComboBox();
  snappingBox->setToolTip("Snap to Grid Setting");
  connect(snappingBox,qOverload<int>(&QComboBox::currentIndexChanged),this,&MapWidget::snappingSelected);
  snappingBox->insertItem(0,"No Snapping",QVariant(0.0));
  snappingBox->insertItem(1,"1 Grid Cell",QVariant(1.0));
  snappingBox->insertItem(2,"1/2 Grid Cell",QVariant(2.0));
  snappingBox->insertItem(3,"1/4 Grid Cell",QVariant(4.0));
  snappingBox->setCurrentIndex(2);


  //populate button layout
  buttonLayout->addWidget(measureButton);
  buttonLayout->addWidget(cursorButton);
  buttonLayout->addWidget(pencilButton);
  buttonLayout->addWidget(lineButton);
  buttonLayout->addWidget(rectangleButton);
  buttonLayout->addWidget(textButton);
  buttonLayout->addWidget(eraserButton);
  buttonLayout->addWidget(brushSizeBox);
  buttonLayout->addWidget(colorButton);
  buttonLayout->addWidget(snappingBox);
  buttonLayout->addWidget(clearfogButton);
  buttonLayout->addWidget(fogButton);
  //buttonLayout->addWidget(new QWidget(),1);
  buttonLayout->addWidget(miniatureButton);
  buttonLayout->addWidget(mapSettingsButton);
  buttonLayout->setAlignment(Qt::AlignLeft);

  //layout stuff
  mapMenuSplitter->addWidget(miniatureMenu);
  mapMenuSplitter->addWidget(mapSettingsWidget);
  mapMenuSplitter->addWidget(view);
  mapMenuSplitter->addWidget(initiativeWidget);
  mapMenuSplitter->setCollapsible(3,false);
  mapMenuSplitter->setSizes({1,1,1000,1});
  miniatureMenu->hide();
  mapSettingsWidget->hide();
  mainLayout->addWidget(mapMenuSplitter);
  mainLayout->addLayout(buttonLayout);

  measureButton->setFlat(true);
  cursorButton->setFlat(true);
  pencilButton->setFlat(true);
  lineButton->setFlat(true);
  eraserButton->setFlat(true);
  rectangleButton->setFlat(true);
  textButton->setFlat(true);
  fogButton->setFlat(true);
  clearfogButton->setFlat(true);
  miniatureButton->setFlat(true);
  mapSettingsButton->setFlat(true);

  //miniature signal
  connect(miniatureMenu,&MiniatureMenuWidget::previewData,scene,&MapScene::setMiniaturePreview);
  connect(scene,&MapScene::miniatureExported,miniatureMenu,&MiniatureMenuWidget::miniatureImport);

  pencilButtonSlot();
  setClientStatus(ROLE_GM);
}

void MapWidget::measureButtonSlot()
{
  resetToolButtons();
  measureButton->setDown(true);
  scene->setTool(TOOL_MEASURE);
}

void MapWidget::cursorButtonSlot()
{
  resetToolButtons();
  cursorButton->setDown(true);
  scene->setTool(TOOL_POINTER);
}

void MapWidget::pencilButtonSlot()
{
  resetToolButtons();
  pencilButton->setDown(true);
  scene->setTool(TOOL_PENCIL);
}

void MapWidget::eraserButtonSlot()
{
  resetToolButtons();
  eraserButton->setDown(true);
  scene->setTool(TOOL_ERASER);
}

void MapWidget::lineButtonSlot()
{
  resetToolButtons();
  lineButton->setDown(true);
  scene->setTool(TOOL_LINE);
}

void MapWidget::rectangleButtonSlot()
{
  resetToolButtons();
  rectangleButton->setDown(true);
  scene->setTool(TOOL_RECTANGLE);
}

void MapWidget::textButtonSlot()
{
  resetToolButtons();
  textButton->setDown(true);
  scene->setTool(TOOL_TEXT);
}

void MapWidget::fogButtonSlot()
{
  resetToolButtons();
  fogButton->setDown(true);
  scene->setTool(TOOL_FOG);
}

void MapWidget::clearfogButtonSlot()
{
  resetToolButtons();
  clearfogButton->setDown(true);
  scene->setTool(TOOL_CLEARFOG);
}
int ctr = 0;

void MapWidget::miniatureButtonSlot()
{
  if(miniatureMenu->isHidden())
  {
    resetToolButtons();
    miniatureMenu->show();
    miniatureButton->setDown(true);
  }
  else
    resetToolButtons();

  mapSettingsWidget->hide();
  mapSettingsButton->setDown(false);
  scene->setTool(TOOL_MINIATURE);
}

void MapWidget::mapSettingsButtonSlot()
{
  if(mapSettingsWidget->isHidden())
  {
    mapSettingsWidget->resetValues();
    mapSettingsWidget->show();
    mapSettingsButton->setDown(true);
  }
  else
  {
    mapSettingsWidget->hide();
    mapSettingsButton->setDown(false);
  }
  miniatureMenu->hide();
  miniatureButton->setDown(false);
}

void MapWidget::snappingSelected(int index)
{
  QVariant variant = snappingBox->itemData(index);
  bool ok;
  double value = variant.toDouble(&ok);
  if(ok)
    scene->setSnapping(value);
}

void MapWidget::setClientStatus(PlayerRole status)
{
  resetToolButtons();
  scene->setTool(TOOL_NONE);
  scene->setStatus(status);
  this->cStatus = status;
  switch(status)
  {
    case ROLE_NOTSET:
    case ROLE_NOINTRODUCTION:
      setDisabledDrawingButtons(false);
      setDisabledFogButtons(false);
      break;
    case ROLE_LURKER:
      setDisabledDrawingButtons(true);
      setDisabledFogButtons(true);
      break;
    case ROLE_PLAYER:
      setDisabledDrawingButtons(false);
      setDisabledFogButtons(true);
      break;
    case ROLE_GM:
    case ROLE_ADMIN:
    case ROLE_INTERNAL:
      setDisabledDrawingButtons(false);
      setDisabledFogButtons(false);
      break;
  }
}

void MapWidget::showInitiative(bool value)
{
  if(value)
    initiativeWidget->show();
  else
    initiativeWidget->hide();
}
