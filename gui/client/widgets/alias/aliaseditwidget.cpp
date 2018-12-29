#include "aliaseditwidget.h"

AliasEditWidget::AliasEditWidget(QWidget *parent) : QWidget(parent)
{
  alias = nullptr;
  layout = new QFormLayout(this);
  layout->setMargin(0);
  nameEdit = new QLineEdit(this);
  colorButton = new ColorSelectButton(this);
  colorLabel = new QLabel("Your text will look like this.",this);
  colorLabel->setMaximumHeight(30);
  aliasSheet = new AliasSheetEditWidget(this);

  layout->addRow("Name:",nameEdit);
  layout->addRow("Color:",colorButton);
  layout->addRow("",colorLabel);
  layout->addRow(aliasSheet);
  QSizePolicy policy = aliasSheet->sizePolicy();
  policy.setVerticalStretch(1);
  aliasSheet->setSizePolicy(policy);

  connect(nameEdit,&QLineEdit::textChanged,this,&AliasEditWidget::nameEditChanged);
  connect(colorButton,&ColorSelectButton::finalColorChanged,this,&AliasEditWidget::colorChanged);
  connect(colorButton,&ColorSelectButton::colorChanged,this,&AliasEditWidget::colorChangedTemporary);
  connect(aliasSheet,&AliasSheetEditWidget::somethingChanged,this,&AliasEditWidget::changed);
}

void AliasEditWidget::setAlias(Alias *alias)
{
  this->alias = alias;
  colorButton->setColor(QColor(alias->getColor()));
  nameEdit->setText(alias->getName());
  aliasSheet->setAlias(alias);
}

void AliasEditWidget::setNightMode(bool value)
{
  aliasSheet->setNightMode(value);
}

void AliasEditWidget::nameEditChanged(const QString& name)
{
  if(alias!=nullptr)
  {
    alias->setName(name);
    emit nameChanged(name);
    emit changed();
  }
}

void AliasEditWidget::colorChanged(const QColor &color)
{
  if(alias!=nullptr)
  {
    alias->setColor(color.name());
    emit changed();
  }
}

void AliasEditWidget::colorChangedTemporary(const QColor &color)
{
  colorLabel->setStyleSheet("QLabel { border: 1px solid;background-color : white; color : "+color.name()+"; }");
}
