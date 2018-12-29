#include "fileselectwidget.h"

QString FileSelectWidget::getDefaultFolder() const
{
  return defaultFolder;
}

void FileSelectWidget::setDefaultFolder(const QString &value)
{
  defaultFolder = value;
}

void FileSelectWidget::updateStyleSheet()
{
  if(!existingFileOnly || QFile(text()).exists())
  {
    edit->setToolTip("");
    edit->setStyleSheet("");
  }
  else
  {
    edit->setToolTip("File does not exist");
    edit->setStyleSheet("color: #FF0000");
  }
}

FileSelectWidget::FileSelectWidget(bool existingFileOnly, QWidget *parent) : QWidget(parent)
{
  this->existingFileOnly = existingFileOnly;
  layout = new QHBoxLayout(this);
  browseButton = new QPushButton("Browse");
  edit = new QLineEdit();
  layout->addWidget(edit,1);
  layout->addWidget(browseButton,0);
  layout->setMargin(0);

  connect(edit,&QLineEdit::textChanged,this,&FileSelectWidget::textChangedSlot);
  connect(browseButton,&QPushButton::clicked,this,&FileSelectWidget::browseButtonClicked);
}

QString FileSelectWidget::text() const
{
  return edit->text();
}

QString FileSelectWidget::placeholderText() const
{
  return edit->placeholderText();
}

void FileSelectWidget::setPlaceholderText(const QString &text)
{
  edit->setPlaceholderText(text);
}

void FileSelectWidget::setText(const QString &text)
{
  edit->setText(text);
}

void FileSelectWidget::textChangedSlot(const QString &text)
{
  updateStyleSheet();
  emit textChanged(text);
}

void FileSelectWidget::browseButtonClicked()
{
  QString defaultFolder = edit->text();
  if(defaultFolder.isEmpty())
    defaultFolder = this->defaultFolder;
  if(existingFileOnly)
  {
    QString filename;
    filename = QFileDialog::getOpenFileName(this,
                                            "Browse",
                                            defaultFolder,
                                            "PEM-File (*.pem);;Any Files (*)");
    if(!filename.isEmpty())
      setText(filename);
  }
  else
  {
    QString filename;
    filename = QFileDialog::getSaveFileName(this,
                                            "Browse",
                                            defaultFolder,
                                            "PEM-File (*.pem);;Any Files (*)");
    if(!filename.isEmpty())
      setText(filename);
  }
}
