#include "imageurlselection.h"
#include "gui/util/filehashurltable.h"

#include <QHBoxLayout>
#include <QProgressBar>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QImgurUploader>
#include <QImgurReply>
#include <QMessageBox>
#include <QJsonArray>

#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>

void ImageUrlSelection::dragEnterEvent(QDragEnterEvent* event)
{
  if(event->mimeData()->urls().size()==1)
  {
    QString filepath = event->mimeData()->urls().first().toLocalFile();
    if(filepath.endsWith(".png") ||
       filepath.endsWith(".apng") ||
       filepath.endsWith(".gif") ||
       filepath.endsWith(".jpg") ||
       filepath.endsWith(".jpeg") ||
       filepath.endsWith(".bmp"))
    {
      event->acceptProposedAction();
    }
  }
}

void ImageUrlSelection::dropEvent(QDropEvent* event)
{
  if(event->mimeData()->urls().size()==1)
  {
    QString filepath = event->mimeData()->urls().first().toLocalFile();
    upload(filepath);
  }
}

ImageUrlSelection::ImageUrlSelection(const QString& clientID, QWidget *parent) : QWidget(parent)
{
  setAcceptDrops(true);
  m_layout = new QHBoxLayout(this);
  m_layout->setMargin(0);
  m_lineEdit = new QLineEdit(this);
  m_uploadButton = new QPushButton("Upload",this);
  m_uploadButton->setToolTip("Upload a local image file to Imgur");
  m_progressBar = new QProgressBar(this);
  m_layout->addWidget(m_lineEdit,1);
  m_layout->addWidget(m_uploadButton,0);
  m_layout->addWidget(m_progressBar,1);
  m_progressBar->setHidden(true);
  m_reply = nullptr;
  m_uploader = new QImgurUploader(clientID,this);

  connect(m_uploadButton,&QPushButton::clicked,this,&ImageUrlSelection::uploadClicked);
}

QProgressBar *ImageUrlSelection::progressBar() const
{
  return m_progressBar;
}

QPushButton *ImageUrlSelection::uploadButton() const
{
  return m_uploadButton;
}

QLineEdit *ImageUrlSelection::lineEdit() const
{
  return m_lineEdit;
}

QString ImageUrlSelection::text()
{
  return m_lineEdit->text();
}

bool ImageUrlSelection::finished()
{
  return m_progressBar->isHidden();
}

QImgurUploader *ImageUrlSelection::uploader() const
{
  return m_uploader;
}

void ImageUrlSelection::setText(const QString &text)
{
  m_lineEdit->setText(text);
}

void ImageUrlSelection::clear()
{
  m_lineEdit->clear();
}

void ImageUrlSelection::upload(const QString& filepath)
{
  if(filepath.isEmpty())
    return;
  QFile file(filepath);
  if(file.open(QIODevice::ReadOnly))
  {
    m_lastHash = FileHashUrlTable::hash(&file);
    file.close();
    if(FileHashUrlTable::get().contains(m_lastHash))
    {
      m_lineEdit->setText(FileHashUrlTable::get()[m_lastHash]);
    }
    else
    {
      m_reply = m_uploader->upload(filepath);
      connect(m_reply,&QImgurReply::finished,this,&ImageUrlSelection::uploadFinished);
      connect(m_reply,&QImgurReply::uploadProgress,this,&ImageUrlSelection::uploadProgress);
      uploadProgress(0,1);
      m_lineEdit->setHidden(true);
      m_uploadButton->setHidden(true);
      m_progressBar->setHidden(false);
    }
  }
  else
  {
    QMessageBox::warning(this,"Upload Error","Could not open '"+filepath,+"'.");
  }
}

void ImageUrlSelection::uploadClicked()
{
  QString filepath = QFileDialog::getOpenFileName(this,"Select Image File",QString(),"Image Files (*.png *.apng  *.gif *.jpg *.jpeg *.bmp);;All Files (*)");
  upload(filepath);
}

void ImageUrlSelection::uploadFinished()
{
  if(m_reply->success())
  {
    FileHashUrlTable::get().insert(m_lastHash,m_reply->link());
    m_lineEdit->setText(m_reply->link());
  }
  else
  {
    QMessageBox::warning(this,"Upload Error",m_reply->errorString());
  }
  m_reply->deleteLater();
  m_reply = nullptr;
  m_lineEdit->setHidden(false);
  m_uploadButton->setHidden(false);
  m_progressBar->setHidden(true);
}

void ImageUrlSelection::uploadProgress(quint64 sent, quint64 total)
{
  m_progressBar->setMaximum(total);
  m_progressBar->setValue(sent);
}
