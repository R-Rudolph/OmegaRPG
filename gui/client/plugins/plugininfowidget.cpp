#include "plugininfowidget.h"

PluginInfoWidget::PluginInfoWidget(QWidget *parent) : QWidget(parent)
{
  layout_ = new QStackedLayout(this);
  QWidget* mainWidget = new QWidget(this);
  QWidget* errorWidget = new QWidget(this);
  layout_->addWidget(mainWidget);
  layout_->addWidget(errorWidget);
  errorLayout_ = new QVBoxLayout(errorWidget);
  mainLayout_ = new QFormLayout(mainWidget);

  nameLabel_ = new QLabel();
  descriptionText_ = new QTextBrowser();
  errorsText_ = new QTextBrowser();
  errorsText_->setMinimumHeight(30);
  errorsText_->setMinimumHeight(50);

  mainLayout_->addRow("Name",nameLabel_);
  mainLayout_->addRow("Description",descriptionText_);
  errorLayout_->addWidget(new QLabel("Errors:",this));
  errorLayout_->addWidget(errorsText_);
}

void PluginInfoWidget::setInfo(const PluginInfo& info)
{
  if(info.isValid())
  {
    nameLabel_->setText(info.name());
    descriptionText_->setText(info.description().isEmpty() ? "<i>No description.</i>" : info.description()  );
    QString errorString;
    foreach(const QString& error, info.errors())
    {
      errorString.append(error);
      errorString.append("\n");
    }
    if(!errorString.isEmpty())
    {
      errorsText_->setText(errorString);
      layout_->setCurrentIndex(1);
    }
    else
      layout_->setCurrentIndex(0);
  }
  else
  {
    QString errorString;
    foreach(const QString& error, info.errors())
    {
      errorString.append(error);
      errorString.append("\n");
    }
    if(errorString.isEmpty())
      errorString = "Unknown error. Could not load plugin.";
    errorsText_->setText(errorString);
    layout_->setCurrentIndex(0);
  }
  nameLabel_->setDisabled(nameLabel_->text().isEmpty());
}
