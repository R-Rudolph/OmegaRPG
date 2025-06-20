#include "miniatureeditdialog.h"

#include "widgets/miniaturesizespinbox.h"

QString MiniatureEditDialog::getName() const
{
  return nameEdit->text();
}

void MiniatureEditDialog::setName(const QString &value)
{
  nameEdit->setText(value);
}

QString MiniatureEditDialog::getUrl() const
{
  if(getUrlMode())
  {
    return urlEdit->text();
  }
  else
  {
    return colorButton->getColor().name();
  }
}

void MiniatureEditDialog::setUrl(const QString &value)
{
  QColor color(value);
  if(color.isValid())
  {
    setUrlMode(false);
    colorButton->setColor(color);
  }
  else
  {
    setUrlMode(true);
    urlEdit->setText(value);
  }
}

qreal MiniatureEditDialog::getSize() const
{
  return sizeBox->value();
}

void MiniatureEditDialog::setSize(const qreal &value)
{
  sizeBox->setValue(value);
}

int MiniatureEditDialog::getId() const
{
  return miniature.getID();
}

void MiniatureEditDialog::setId(int value)
{
  miniature.setID(value);
}

void MiniatureEditDialog::setComment(const QString &value)
{
  commentEdit->setText(value);
}

QString MiniatureEditDialog::getComment() const
{
  return commentEdit->text();
}

void MiniatureEditDialog::setMiniatureMenuMode(bool value)
{
  if(value)
  {
    setWidgetHidden(commentEdit,false);
    setWidgetHidden(rotationBox,true);
    setWidgetHidden(rotationSpinBox,true);
    setWidgetHidden(directionBox,true);
    setWidgetHidden(directionSpinBox,true);
  }
  else
  {
    setWidgetHidden(commentEdit,true);
    setWidgetHidden(rotationBox,false);
    setWidgetHidden(rotationSpinBox,false);
    setWidgetHidden(directionBox,false);
    setWidgetHidden(directionSpinBox,false);
  }
}

void MiniatureEditDialog::setLayer(MapMiniatureResource::Layer value)
{
  if(value==MapMiniatureResource::LAYER_ABOVELINES)
    layerBox->setCurrentIndex(0);
  else if(value==MapMiniatureResource::LAYER_BELOWLINES)
    layerBox->setCurrentIndex(1);
  else if(value==MapMiniatureResource::LAYER_BACKGROUND)
    layerBox->setCurrentIndex(2);
  else
    layerBox->setCurrentIndex(3);
}

MapMiniatureResource::Layer MiniatureEditDialog::getLayer() const
{
  return (MapMiniatureResource::Layer) layerBox->currentData().toInt();
}

void MiniatureEditDialog::setDisplay(MapMiniatureResource::Display value)
{
  switch(value)
  {
    case MapMiniatureResource::DISPLAY_CIRCLED:
      displayBox->setCurrentIndex(1);
      break;
    default:
      displayBox->setCurrentIndex(0);
      break;
  }
}

MapMiniatureResource::Display MiniatureEditDialog::getDisplay() const
{
  return (MapMiniatureResource::Display) displayBox->currentData().toInt();
}

void MiniatureEditDialog::setVisibility(MapMiniatureResource::Visibility value)
{
  visibilityBox->setCurrentIndex((int)value);
}

MapMiniatureResource::Visibility MiniatureEditDialog::getVisibility() const
{
  return (MapMiniatureResource::Visibility) visibilityBox->currentIndex();
}

void MiniatureEditDialog::okSlot()
{
  accept();
}

void MiniatureEditDialog::sourceChanged(int index)
{
  setUrlMode(index==0);
}

void MiniatureEditDialog::rotationBoxChanged(int index)
{
  if(index == rotationBox->count()-1)
  {
    rotationSpinBox->setDisabled(false);
  }
  else
  {
    rotationSpinBox->setValue(rotationBox->itemData(index).toReal());
    rotationSpinBox->setDisabled(true);
  }
}

void MiniatureEditDialog::directionBoxChanged(int index)
{
  if(index == directionBox->count()-1)
  {
    directionSpinBox->setDisabled(false);
  }
  else
  {
    directionSpinBox->setValue(directionBox->itemData(index).toReal());
    directionSpinBox->setDisabled(true);
  }
}

void MiniatureEditDialog::setUrlMode(bool url)
{
  if(url)
  {
    if(imageSourceBox->currentIndex()!=0)
      imageSourceBox->setCurrentIndex(0);
    setWidgetHidden(colorButton,true);
    setWidgetHidden(urlEdit,false);
  }
  else
  {
    if(imageSourceBox->currentIndex()!=1)
      imageSourceBox->setCurrentIndex(1);
    setWidgetHidden(urlEdit,true);
    setWidgetHidden(colorButton,false);
  }
}

bool MiniatureEditDialog::getUrlMode() const
{
  return imageSourceBox->currentIndex()==0;
}

void MiniatureEditDialog::addRow(const QString &label, QWidget *widget)
{
  int row = layout->rowCount();
  if(!label.isEmpty())
  {
    QLabel* labelWidget = new QLabel(label);
    labelMap.insert(widget,labelWidget);
    layout->addWidget(labelWidget,row,0,1,1);
  }
  layout->addWidget(widget,row,1,1,1);
}

void MiniatureEditDialog::addRow(QWidget *widget)
{
  layout->addWidget(widget,layout->rowCount(),0,1,2);
}

void MiniatureEditDialog::addRow(QLayout *layout)
{
    this->layout->addLayout(layout,this->layout->rowCount(),0,1,2);
}

void MiniatureEditDialog::addRow(const QString &label, QLayout *layout)
{
    this->layout->addLayout(layout,this->layout->rowCount(),0,1,2);

    int row = this->layout->rowCount();
    if(!label.isEmpty())
    {
        QLabel* labelWidget = new QLabel(label);
        labelMap.insert((QWidget*)layout,labelWidget);
        this->layout->addWidget(labelWidget,row,0,1,1);
    }
    this->layout->addLayout(layout,row,1,1,1);
}

void MiniatureEditDialog::setWidgetHidden(QWidget *widget, bool value)
{
  if(labelMap.contains(widget))
  {
    if(value)
      labelMap[widget]->hide();
    else
      labelMap[widget]->show();
  }
  if(value)
    widget->hide();
  else
    widget->show();
}

void MiniatureEditDialog::setRotation(qreal deg)
{
  rotationSpinBox->setValue(deg);
  rotationSpinBox->setDisabled(true);
  for(int i=0;i<rotationBox->count();i++)
  {
    if(rotationBox->itemData(i).toReal()==deg)
    {
      rotationBox->setCurrentIndex(i);
      return;
    }
  }
  rotationSpinBox->setDisabled(false);
  rotationBox->setCurrentIndex(rotationBox->count()-1);
}

void MiniatureEditDialog::setDirection(qreal deg)
{
  directionSpinBox->setValue(deg);
  directionSpinBox->setDisabled(true);
  for(int i=0;i<directionBox->count();i++)
  {
    if(directionBox->itemData(i).toReal()==deg)
    {
      directionBox->setCurrentIndex(i);
      return;
    }
  }
  directionSpinBox->setDisabled(false);
  directionBox->setCurrentIndex(directionBox->count()-1);
}

void MiniatureEditDialog::graphicsSliderChanged(int value) {
  this->graphicStateSliderLabel->setText(QString::fromStdString(std::to_string(value+1)));
  setUrl(urls[value]);
}

void MiniatureEditDialog::urlLineEditChanged(const QString& value) {
  this->urls[this->graphicStateSlider->value()] = value;
}

void MiniatureEditDialog::colorChanged(QColor value) {
  this->urls[this->graphicStateSlider->value()] = value.name();
}

MiniatureEditDialog::MiniatureEditDialog(QWidget* parent) : NightModeDialog(parent)
{
  urls = QVector<QString>(8);
  layout = new QGridLayout(getCentralWidget());
  nameEdit = new QLineEdit();
  urlEdit  = new ImageUrlSelection(Global::imgurClientID());
  sizeBox = new MiniatureSizeSpinBox();
  sizeBox->setMinimum(1.0/4.0);
  sizeBox->setMaximum(1000);
  sizeBox->setSuffix(" Grid Cells");
  graphicStateSlider = new QSlider(this);
  quint32 selectedState = 0;
  graphicStateSlider->setValue(selectedState);
  graphicStateSlider->setMinimum(0);
  graphicStateSlider->setMaximum(7);
  graphicStateSlider->setOrientation(Qt::Horizontal);
  graphicStateSliderLabel = new QLabel(QString::fromStdString(std::to_string(selectedState+1)), this);
  QHBoxLayout* graphicStateLayout = new QHBoxLayout(this);
  QWidget* fuckoff = new QWidget(this);
  fuckoff->setLayout(graphicStateLayout);
  graphicStateLayout->addWidget(graphicStateSliderLabel);
  graphicStateLayout->addWidget(graphicStateSlider);
  imageSourceBox = new QComboBox(this);
  imageSourceBox->addItem("URL");
  imageSourceBox->addItem("Color");
  colorButton = new ColorSelectButton(this);

  layerBox = new QComboBox(this);
  layerBox->addItem("Above Lines",MapMiniatureResource::LAYER_ABOVELINES);
  layerBox->addItem("Below Lines",MapMiniatureResource::LAYER_BELOWLINES);
  layerBox->addItem("Background",MapMiniatureResource::LAYER_BACKGROUND);
  layerBox->addItem("Above Fog",MapMiniatureResource::LAYER_ABOVEFOG);

  displayBox = new QComboBox();
  displayBox->addItem("Normal",MapMiniatureResource::DISPLAY_NORMAL);
  displayBox->addItem("Circled",MapMiniatureResource::DISPLAY_CIRCLED);

  visibilityBox = new QComboBox();
  visibilityBox->addItem("Visible",MapMiniatureResource::VISIBILITY_VISIBLE);
  visibilityBox->addItem("Partially Visible",MapMiniatureResource::VISIBILITY_PARTIAL);
  visibilityBox->addItem("Invisible",MapMiniatureResource::VISIBILITY_INVISIBLE);

  commentEdit = new QLineEdit();


  rotationBox = new QComboBox(this);
  rotationBox->setToolTip("Set the rotation of the miniature image.");
  rotationBox->addItem("North",0.0f);
  rotationBox->addItem("East",90.0f);
  rotationBox->addItem("South",180.0f);
  rotationBox->addItem("West",270.0f);
  rotationBox->addItem("Custom");
  rotationSpinBox = new QSpinBox(this);
  rotationSpinBox->setToolTip("Numeric representation of the rotation of the miniature image. (0° to 360°).");
  rotationSpinBox->setMinimum(0);
  rotationSpinBox->setMaximum(360);

  directionBox = new QComboBox(this);
  directionBox->setToolTip("Set the indication of direction for this miniature.");
  directionBox->addItem("None",-1.0f);
  directionBox->addItem("North",0.0);
  directionBox->addItem("North-East",45.0f);
  directionBox->addItem("East",90.0f);
  directionBox->addItem("South-East",135.0f);
  directionBox->addItem("South",180.0f);
  directionBox->addItem("South-West",225.0f);
  directionBox->addItem("West",270.0f);
  directionBox->addItem("North-West",315.0f);
  directionBox->addItem("Custom");
  directionSpinBox = new QSpinBox(this);
  directionSpinBox->setToolTip("Numeric representation of the indication of direction for this miniature. (0° to 360°, -1 to hide)");
  directionSpinBox->setSpecialValueText("No Direction");
  directionSpinBox->setMinimum(-1);
  directionSpinBox->setMaximum(360);

  okButton = new QPushButton("Ok");
  cancelButton = new QPushButton("Cancel");
  buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);

  addRow("Name:",nameEdit);
  addRow("State:",fuckoff);
  addRow("Image Source:",imageSourceBox);
  addRow("URL:",urlEdit);
  addRow("Color:",colorButton);
  addRow("Size:",sizeBox);
  addRow("Layer:",layerBox);
  addRow("Visibility:",visibilityBox);
  addRow("Effect:",displayBox);
  addRow("Comment",commentEdit);
  addRow("Rotation",rotationBox);
  addRow("",rotationSpinBox);
  addRow("Direction",directionBox);
  addRow("",directionSpinBox);
  addRow(buttonLayout);

  setMiniatureMenuMode(false);

  setSize(1);
  setUrl("");

  connect(okButton,&QPushButton::clicked,this,&MiniatureEditDialog::okSlot);
  connect(cancelButton,&QAbstractButton::clicked,this,&QDialog::reject);
  connect(imageSourceBox,qOverload<int>(&QComboBox::currentIndexChanged), this, &MiniatureEditDialog::sourceChanged);
  connect(rotationBox,qOverload<int>(&QComboBox::currentIndexChanged),this,&MiniatureEditDialog::rotationBoxChanged);
  connect(directionBox,qOverload<int>(&QComboBox::currentIndexChanged),this,&MiniatureEditDialog::directionBoxChanged);
  connect(graphicStateSlider, qOverload<int>(&QSlider::valueChanged), this, &MiniatureEditDialog::graphicsSliderChanged);
  connect(this->urlEdit->lineEdit(), qOverload<const QString&>(&QLineEdit::textChanged), this, &MiniatureEditDialog::urlLineEditChanged);
  connect(this->colorButton, qOverload<QColor>(&ColorSelectButton::finalColorChanged), this, &MiniatureEditDialog::colorChanged);
}

MapMiniatureResource MiniatureEditDialog::getMiniature()
{
  miniature.setName(nameEdit->text());
  QVector<QString> urls;
  for (int i = 0; i<this->urls.size(); ++i) {
    if (this->urls[i].size()>0) {
      urls.push_back(this->urls[i]);
    }
  }
  miniature.setSelectedGraphicsIndex(this->graphicStateSlider->value());
  miniature.setGraphic(urls);
  miniature.setSize(getSize());
  miniature.setLayer(getLayer());
  miniature.setVisibility(getVisibility());
  miniature.setDisplay(getDisplay());
  miniature.setRotation(rotationSpinBox->value());
  miniature.setDirection(directionSpinBox->value());
  return miniature;
}

void MiniatureEditDialog::setMiniature(const MapMiniatureResource &mini)
{
  miniature = mini;
  setName(mini.getName());
  auto graphics = mini.getGraphic();
  for(int i=0; i<std::min(this->urls.size(),graphics.size()); ++i) {
      this->urls[i] = graphics[i];
  }
  //setUrl(mini.getGraphic()[0]);
  setSize(mini.getSize());
  setLayer(mini.getLayer());
  setVisibility(mini.getVisibility());
  setDisplay(mini.getDisplay());
  setRotation(mini.getRotation());
  setDirection(mini.getDirection());
  auto selectedIndex = mini.getSelectedGraphicsIndex();
  if (selectedIndex > 0 && selectedIndex < 8) {
    graphicsSliderChanged(mini.getSelectedGraphicsIndex());
    setUrl(this->urls[mini.getSelectedGraphicsIndex()]);
  } else {
      graphicsSliderChanged(0);
      setUrl(this->urls[0]);
  }
}
