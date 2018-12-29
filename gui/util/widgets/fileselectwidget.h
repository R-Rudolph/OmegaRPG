#ifndef FILESELECTWIDGET_H
#define FILESELECTWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QFile>
#include <QDir>

class FileSelectWidget : public QWidget
{
  Q_OBJECT
  QString defaultFolder;
  bool existingFileOnly;
  QHBoxLayout* layout;
  QPushButton* browseButton;
  QLineEdit* edit;
  void updateStyleSheet();
public:
  explicit FileSelectWidget(bool existingFileOnly=false,QWidget *parent = 0);
  QString text() const;
  QString placeholderText() const;
  QString getDefaultFolder() const;
signals:
  void textChanged(const QString& text);
public slots:
  void setDefaultFolder(const QString &value);
  void setPlaceholderText(const QString& text);
  void setText(const QString& text);
private slots:
  void textChangedSlot(const QString& text);
  void browseButtonClicked();
};

#endif // FILESELECTWIDGET_H
