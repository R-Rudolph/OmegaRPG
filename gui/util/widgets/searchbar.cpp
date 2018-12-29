#include "searchbar.h"

const QString SearchBar::prevNextStyleSheet = "QPushButton"
                                              "{"
                                              "margin-left:-20px;"
                                              "margin-right:-20px;"
                                              "}";

void SearchBar::sendSearch(bool backwards)
{
  emit search(stringEdit->text(),regEx,backwards,caseSensitiveSwitch->isChecked(),regularExpressionSwitch->isChecked(),wholeWordsSwitch->isChecked());
}

SearchBar::SearchBar(QWidget *parent) : QWidget(parent)
{
  layout = new QHBoxLayout(this);
  layout->setMargin(0);
  stringEdit = new QLineEdit(this);
  nextButton = new QPushButton("▼",this);
  prevButton = new QPushButton("▲",this);
  nextButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
  prevButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
  nextButton->setMinimumWidth(25);
  prevButton->setMinimumWidth(25);
  nextButton->setMaximumWidth(25);
  prevButton->setMaximumWidth(25);
  nextButton->setStyleSheet(prevNextStyleSheet);
  prevButton->setStyleSheet(prevNextStyleSheet);
  caseSensitiveSwitch = new QPushButton("Match Case",this);
  wholeWordsSwitch = new QPushButton("Whole Words");
  regularExpressionSwitch = new QPushButton("Reg. Expr.");
  caseSensitiveSwitch->setCheckable(true);
  wholeWordsSwitch->setCheckable(true);
  regularExpressionSwitch->setCheckable(true);
  hideButton = new QPushButton("✖",this);

  hideButton->setStyleSheet("QPushButton"
                            "{"
                            "border-style: outset;"
                            "border-width: 2px;"
                            "border-radius: 10px;"
                            "border-color: beige;"
                            "padding: 3px;"
                            "}"
                            "QPushButton:hover"
                            "{"
                            "font-weight: bold;"
                            "background-color: #ff8080;"
                            "}"
                            "QPushButton:pressed"
                            "{"
                            "background-color: #ff4040;"
                            "border-style: inset;"
                            "}");

  layout->addWidget(stringEdit);
  layout->addWidget(prevButton);
  layout->addWidget(nextButton);
  layout->addStretch(1);
  layout->addWidget(caseSensitiveSwitch);
  layout->addWidget(wholeWordsSwitch);
  layout->addWidget(regularExpressionSwitch);
  layout->addWidget(hideButton);

  connect(hideButton,&QPushButton::clicked,this,&QWidget::hide);
  connect(stringEdit,&QLineEdit::textChanged,this,&SearchBar::updateInfo);
  connect(wholeWordsSwitch,&QPushButton::clicked,this,&SearchBar::updateInfo);
  connect(regularExpressionSwitch,&QPushButton::clicked,this,&SearchBar::updateInfo);

  connect(stringEdit,&QLineEdit::returnPressed,this,&SearchBar::searchNext);
  connect(nextButton,&QPushButton::clicked,this,&SearchBar::searchNext);
  connect(prevButton,&QPushButton::clicked,this,&SearchBar::searchPrev);
}

void SearchBar::focus()
{
  stringEdit->setFocus();
}

void SearchBar::updateInfo()
{
  if(caseSensitiveSwitch->isChecked())
    regEx = QRegExp(stringEdit->text());
  else
    regEx = QRegExp(stringEdit->text(),Qt::CaseInsensitive);
  if(regularExpressionSwitch->isChecked() && !regEx.isValid())
    stringEdit->setStyleSheet("color:#ff0000;");
  else
    stringEdit->setStyleSheet("");
}

void SearchBar::searchNext()
{
  sendSearch(false);
}

void SearchBar::searchPrev()
{
  sendSearch(true);
}
