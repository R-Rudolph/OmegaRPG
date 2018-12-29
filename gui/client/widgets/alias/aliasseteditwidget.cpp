#include "aliasseteditwidget.h"

AliasSetEditWidget::AliasSetEditWidget(QWidget *parent) : QWidget(parent)
{
  layout = new QFormLayout(this);
  nameEdit = new QLineEdit(this);
  layout->setMargin(0);
  layout->addRow("Name:",nameEdit);
  connect(nameEdit,&QLineEdit::textChanged,this,&AliasSetEditWidget::nameEditChanged);
}

void AliasSetEditWidget::setAliasSet(AliasSet *aliasSet)
{
  this->aliasSet = aliasSet;
  nameEdit->setText(aliasSet->name);
}

void AliasSetEditWidget::nameEditChanged(const QString &name)
{
  if(aliasSet!=nullptr)
  {
    aliasSet->name = name;
    emit nameChanged(name);
    emit changed();
  }
}
