#ifndef FOLDERSELECTWIDGET_H
#define FOLDERSELECTWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QDir>

#include "gui/util/globalgui.h"

class FolderSelectWidget : public QWidget
{
  Q_OBJECT
  QHBoxLayout* hbox;
  QLineEdit* edit;
  QPushButton* button;
  bool selectFile;
public:
  explicit FolderSelectWidget(QWidget *parent = 0, bool selectFile=false);
  QString text();
signals:
  void textChanged(const QString& text);
public slots:
  void setPlaceholderText(const QString& text);
  void editTextChanged(const QString& text);
  void setText(const QString& text);
  void buttonPressed();
};

#endif // FOLDERSELECTWIDGET_H
