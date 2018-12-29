#ifndef IMAGEURLSELECTION_H
#define IMAGEURLSELECTION_H

#include <QWidget>
#include <QMap>

class QHBoxLayout;
class QProgressBar;
class QPushButton;
class QLineEdit;
class QImgurUploader;
class QImgurReply;

class QDragEnterEvent;
class QDragLeaveEvent;
class QDragMoveEvent;
class QDropEvent;

class ImageUrlSelection : public QWidget
{
  Q_OBJECT
  QHBoxLayout* m_layout;
  Q_PROPERTY(QProgressBar* progressBar READ progressBar)
  Q_PROPERTY(QPushButton* uploadButton READ uploadButton)
  Q_PROPERTY(QLineEdit* lineEdit READ lineEdit)
  Q_PROPERTY(QImgurUploader* uploader READ uploader)
  QLineEdit* m_lineEdit;
  QPushButton* m_uploadButton;
  QProgressBar* m_progressBar;
  QImgurUploader* m_uploader;
  QImgurReply* m_reply;

  QByteArray m_lastHash;
protected:
  virtual void 	dragEnterEvent(QDragEnterEvent *event);
  //virtual void 	dragLeaveEvent(QDragLeaveEvent *event);
  //virtual void 	dragMoveEvent(QDragMoveEvent *event);
  virtual void 	dropEvent(QDropEvent *event);
public:
  explicit ImageUrlSelection(const QString& clientID, QWidget *parent = nullptr);

  QProgressBar* progressBar() const;
  QPushButton* uploadButton() const;
  QLineEdit* lineEdit() const;
  QString text();
  bool finished();
  QImgurUploader* uploader() const;

signals:
public slots:
  void setText(const QString& text);
  void clear();

private slots:
  void upload(const QString& filepath);
  void uploadClicked();
  void uploadFinished();
  void uploadProgress(quint64 sent, quint64 total);
};

#endif // IMAGEURLSELECTION_H
