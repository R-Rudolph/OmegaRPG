#include "logfilterdialog.h"

#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QCheckBox>

using namespace orpg;

LogFilterDialog::LogFilterDialog(QWidget *parent) : NightModeDialog(parent)
{
  QFormLayout* layout = new QFormLayout(getCentralWidget());
  okButton = new QPushButton("Ok",this);
  cancelButton = new QPushButton("Cancel",this);
  fieldBox = new QComboBox(this);
  matchBox = new QComboBox(this);
  stringEdit = new QLineEdit(this);
  invertFilterBox = new QComboBox(this);
  buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);

  invertFilterBox->addItem("Must Contain",0);
  invertFilterBox->addItem("Must Not Contain",1);

  std::initializer_list<LogFilter::FilterField> fieldList = {LogFilter::FIELD_ROOM,
                                                              LogFilter::FIELD_ALIAS,
                                                              LogFilter::FIELD_TEXT};
  foreach(LogFilter::FilterField field, QList<LogFilter::FilterField>(fieldList))
  {
    fieldBox->addItem(LogFilter::descriptiveString(field),field);
  }

  std::initializer_list<LogFilter::MatchType> matchList = {LogFilter::MATCH_PARTIAL,
                                                            LogFilter::MATCH_EXACT,
                                                            LogFilter::MATCH_WORDS,
                                                            LogFilter::MATCH_REGEX};
  foreach(LogFilter::MatchType match, QList<LogFilter::MatchType>(matchList))
  {
    matchBox->addItem(LogFilter::descriptiveString(match),match);
  }

  layout->addRow("Field",fieldBox);
  layout->addRow("",invertFilterBox);
  layout->addRow("Match Type",matchBox);
  layout->addRow("Filter",stringEdit);
  layout->addRow(buttonLayout);

  connect(okButton,&QPushButton::clicked,this,&LogFilterDialog::okClicked);
  connect(cancelButton,&QPushButton::clicked,this,&LogFilterDialog::reject);
  connect(stringEdit,&QLineEdit::textChanged,this,&LogFilterDialog::changed);
  connect(matchBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&LogFilterDialog::changed);
}

LogFilter LogFilterDialog::getFilter()
{
  return LogFilter((LogFilter::FilterField)fieldBox->currentData().toInt(),
                   (LogFilter::MatchType)matchBox->currentData().toInt(),
                   stringEdit->text(),
                   invertFilterBox->currentData().toInt());
}

void LogFilterDialog::okClicked()
{
  if(stringEdit->text().isEmpty())
  {
    QMessageBox::warning(this,"Filter Error","No filter string given.",QMessageBox::Ok,QMessageBox::Ok);
    return;
  }
  else if((matchBox->currentData().toInt()==LogFilter::MATCH_REGEX) && !regex.isValid())
  {
    QMessageBox::warning(this,"Filter Error","The given regular expression is invalid.",QMessageBox::Ok,QMessageBox::Ok);
    return;
  }
  accept();
}

void LogFilterDialog::changed()
{
  if(matchBox->currentData().toInt()==LogFilter::MATCH_REGEX)
  {
    regex.setPattern(stringEdit->text());
    if(regex.isValid())
      stringEdit->setStyleSheet("");
    else
      stringEdit->setStyleSheet("color:#FF0000;");
  }
  else
  {
    stringEdit->setStyleSheet("");
  }
}
