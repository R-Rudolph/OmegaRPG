#include "mapsettingswidget.h"
#include "core/client/clientcore.h"
#include "core/multi/roomvariables/mapsettingsvariablecontainer.h"
#include "core/multi/roomvariables/mapsettingsvariable.h"
#include "core/multi/message/roomvariablemessage.h"

MapSettingsWidget::MapSettingsWidget(QWidget *parent) : QScrollArea(parent)
{
  wrapperWidget = new QWidget();
  layout = new QGridLayout(wrapperWidget);
  layout->setMargin(0);
  QHBoxLayout* sizeLayout = new QHBoxLayout();
  //map size stuff
  sizeLabel = new QLabel("Map Size");
  xLabel = new QLabel("x");
  xSizeBox = new QSpinBox();
  ySizeBox = new QSpinBox();
  xSizeBox->setMinimumWidth(70);
  ySizeBox->setMinimumWidth(70);
  sizeLabel->setToolTip("Map Size (in grid cells)");
  xLabel->setToolTip("Map Size (in grid cells)");
  xSizeBox->setToolTip("Horizontal Map Size (in grid cells)");
  xSizeBox->setMaximum(10000);
  ySizeBox->setToolTip("Vertical Map Size (in grid cells)");
  ySizeBox->setMaximum(10000);

  //color stuff
  backgroundColorLabel = new QLabel("Background Color");
  gridColorLabel = new QLabel("Grid Color");
  backgroundColorButton = new ColorSelectButton();
  gridColorButton = new ColorSelectButton();

  //additional
  showGridLabel = new QLabel("Show Grid");
  showGridBox = new QCheckBox();
  showGridBox->setChecked(true);

  lockBackgroundLabel = new QLabel("Lock Background");
  lockBackgroundBox = new QCheckBox();
  lockBackgroundBox->setChecked(false);

  //buttons
  applyButton = new QPushButton("Apply");
  clearButton = new QPushButton("Clear Map");

  //set up layout
  layout->addWidget(sizeLabel,0,0,1,1);
  layout->addLayout(sizeLayout,1,0,1,2);

  sizeLayout->addWidget(xSizeBox);
  sizeLayout->addWidget(xLabel);
  sizeLayout->addWidget(ySizeBox);

  layout->addWidget(backgroundColorLabel,2,0,1,1);
  layout->addWidget(backgroundColorButton,2,1,1,1);
  layout->addWidget(gridColorLabel,3,0,1,1);
  layout->addWidget(gridColorButton,3,1,1,1);
  layout->addWidget(showGridLabel,4,0,1,1);
  layout->addWidget(showGridBox,4,1,1,1);
  layout->addWidget(lockBackgroundLabel,5,0,1,1);
  layout->addWidget(lockBackgroundBox,5,1,1,1);

  layout->addWidget(applyButton,6,0,1,2);
  layout->addWidget(clearButton,7,0,1,2);

  this->setWidget(wrapperWidget);
  this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

  //connections
  connect(applyButton,&QAbstractButton::clicked,this,&MapSettingsWidget::publishSettings);
  connect(clearButton,&QAbstractButton::clicked,this,&MapSettingsWidget::clearMapSlot);
  connect(orpg::ClientCore::get()->getVariables()->getMapSettingsContainer(),&MapSettingsVariableContainer::variableChanged,this,&MapSettingsWidget::loadMapSettings);

  loadMapSettings();
}

int MapSettingsWidget::getWantedWidth()
{
  return this->verticalScrollBar()->width()+wrapperWidget->width();
}

QSize MapSettingsWidget::sizeHint()
{
  QSize size;
  size.setWidth(getWantedWidth());
  return size;
}

void MapSettingsWidget::resetValues()
{
  gridColorButton->setColor(gridColor);
  backgroundColorButton->setColor(bgColor);
  xSizeBox->setValue(sizeX);
  ySizeBox->setValue(sizeY);
  showGridBox->setChecked(showGrid);
  lockBackgroundBox->setChecked(lockBackground);
}

void MapSettingsWidget::publishSettings(bool)
{
  MapSettingsVariable variable = * orpg::ClientCore::get()->getVariables()->getMapSettingsContainer()->getVariable();
  variable.setSizeX(xSizeBox->value());
  variable.setSizeY(ySizeBox->value());
  variable.setBgColor(backgroundColorButton->getColor().name());
  variable.setGridColor(gridColorButton->getColor().name());
  variable.setHideGrid(!showGridBox->isChecked());
  variable.setLockBackground(lockBackgroundBox->isChecked());
  orpg::ClientCore::get()->send(orpg::RoomVariableMessage(variable));
}

void MapSettingsWidget::clearMapSlot(bool)
{
  QMessageBox msgBox(this);
  msgBox.setText("Delete all elements on the map?");
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::Yes);
  if(msgBox.exec()==QMessageBox::Yes)
  {
    orpg::ClientCore::get()->sendClearMapMessage();
  }
}

void MapSettingsWidget::loadMapSettings()
{
  MapSettingsVariable settings = *(orpg::ClientCore::get()->getVariables()->getMapSettingsContainer()->getVariable());
  mapSettingsChanged(settings);
}

void MapSettingsWidget::mapSettingsChanged(const MapSettingsVariable &settings)
{
  sizeX = settings.getSizeX();
  sizeY = settings.getSizeY();
  gridColor = settings.getGridColor();
  bgColor = settings.getBgColor();
  showGrid = !settings.getHideGrid();
  lockBackground = settings.getLockBackground();
  resetValues();
}
