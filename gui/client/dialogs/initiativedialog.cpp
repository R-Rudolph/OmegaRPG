#include "initiativedialog.h"

#include<QVBoxLayout>
#include<QFormLayout>
#include<QHBoxLayout>
#include<QSpinBox>
#include<QLineEdit>
#include<QPushButton>
#include "widgets/imageurlselection.h"
#include "core/multi/roomresources/initiativeresource.h"
#include "widgets/colorselectbutton.h"
#include "core/multi/util/global.h"
#include "core/client/util/dice.h"

InitiativeDialog::InitiativeDialog(QWidget *parent) : NightModeDialog(parent)
{
  setModal(true);
  mainLayout   = new QVBoxLayout(getCentralWidget());
  formLayout   = new QFormLayout();
  buttonLayout = new QHBoxLayout();
  mainLayout->addLayout(formLayout);
  mainLayout->addLayout(buttonLayout);
  initiativeLayout = new QHBoxLayout();

  nameEdit      = new QLineEdit;
  colorButton   = new ColorSelectButton(this,Qt::black);
  iconUrlEdit   = new ImageUrlSelection(Global::imgurClientID(),this);
  initiativeBox = new QSpinBox;
  initiativeRollButton = new QPushButton;
  statusEdit    = new QLineEdit;
  okButton      = new QPushButton("Ok");
  cancelButton  = new QPushButton("Cancel");

  initiativeBox->setMinimum(-1000);
  initiativeBox->setMaximum(1000);

  initiativeRollButton = new QPushButton(this);
  initiativeRollButton->setStyleSheet("QPushButton"
                                      "{"
                                      "color: black invert;"
                                      "border-image: url(:/icons/d20.png) 0 0 0 0 stretch stretch;"
                                      "font-size: 8px;"
                                      "margin-right: 3px;"
                                      "}");
  initiativeRollButton->setText("20");
  initiativeRollButton->setToolTip("Add d20 to value");
  initiativeRollButton->setMinimumHeight(20);
  initiativeRollButton->setMaximumHeight(20);
  initiativeRollButton->setMaximumWidth(20);
  initiativeRollButton->setMinimumWidth(20);
  connect(initiativeRollButton,&QPushButton::clicked,this,&InitiativeDialog::addD20);

  initiativeLayout->addWidget(initiativeBox);
  initiativeLayout->addWidget(initiativeRollButton);

  formLayout->addRow("Name",nameEdit);
  formLayout->addRow("Color",colorButton);
  formLayout->addRow("Icon URL",iconUrlEdit);
  formLayout->addRow("Initiative",initiativeLayout);
  formLayout->addRow("Status",statusEdit);

  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);

  connect(okButton,&QPushButton::clicked,this,&InitiativeDialog::accept);
  connect(cancelButton,&QPushButton::clicked,this,&InitiativeDialog::reject);
}

void InitiativeDialog::setresource(const InitiativeResource &resource)
{
  nameEdit->setText(resource.getName());
  iconUrlEdit->setText(resource.getIconUrl());
  initiativeBox->setValue(resource.getInitiative());
  statusEdit->setText(resource.getStatus());
  colorButton->setColor(QColor(resource.getColor()));
}

InitiativeResource InitiativeDialog::getresource()
{
  return InitiativeResource(nameEdit->text(),statusEdit->text(),iconUrlEdit->text(),false,initiativeBox->value(),0,colorButton->getColor().name());
}

void InitiativeDialog::clear()
{
  nameEdit->clear();
  iconUrlEdit->clear();
  initiativeBox->setValue(0);
  statusEdit->clear();
  colorButton->setColor(Qt::black);
}

int InitiativeDialog::exec()
{
  nameEdit->setFocus();
  return QDialog::exec();
}

void InitiativeDialog::addD20()
{
  initiativeBox->setValue(initiativeBox->value() + Dice::roll(20));
}
