#include "folderselectwidget.h"

FolderSelectWidget::FolderSelectWidget(QWidget *parent, bool selectFile) : QWidget(parent)
{
  this->selectFile = selectFile;
  hbox = new QHBoxLayout(this);
  button = new QPushButton("Browse");
  edit = new QLineEdit();
  hbox->addWidget(edit,1);
  hbox->addWidget(button,0);
  hbox->setMargin(0);
  connect(button,&QPushButton::clicked,this,&FolderSelectWidget::buttonPressed);
  connect(edit,&QLineEdit::textEdited,this,&FolderSelectWidget::editTextChanged);
}

QString FolderSelectWidget::text()
{
  return edit->text();
}

void FolderSelectWidget::setPlaceholderText(const QString &text)
{
  edit->setPlaceholderText(text);
}

void FolderSelectWidget::editTextChanged(const QString &text)
{
  emit textChanged(text);
  bool exists;
  if(selectFile)
  {
    QFile file(text);
    exists = file.exists();
  }
  else
  {
    QDir dir(text);
    exists = dir.exists();
  }
  if(!exists)
  {
    edit->setStyleSheet("color: #FF0000");
  }
  else
  {
    edit->setStyleSheet("color: #000000");
  }
}

void FolderSelectWidget::setText(const QString &text)
{
  edit->setText(text);
  editTextChanged(text);
}

void FolderSelectWidget::buttonPressed()
{
  if(!selectFile)
  {
    QString dir = text();
    if(dir.isEmpty())
      dir = edit->placeholderText();
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setDirectory(dir);
    if(dialog.exec())
    {
      QStringList list = dialog.selectedFiles();
      if(list.size()>0)
        setText(list.at(0));
    }
  }
  else
  {
    QString filepath = QFileDialog::getOpenFileName(this,"Select Audio File",text());
    if(!filepath.isEmpty())
    {
      setText(filepath);
    }
  }
}
