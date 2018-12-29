#ifndef ROOMCREATIONWIDGET_H
#define ROOMCREATIONWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>


class RoomCreationWidget : public QWidget
{
  Q_OBJECT
  //labels
  QLabel* nameLabel;
  QLabel* passLabel;
  QLabel* gmPassLabel;
  //edits
  QLineEdit* nameEdit;
  QLineEdit* passEdit;
  QLineEdit* gmPassEdit;
  //ok button
  QPushButton* createButton;
  //layout stuff
  QVBoxLayout* vbox;
  QGridLayout* grid;
  QWidget* gridWidget;

  //helper functions
  QString randPass(int length) const;
public:
  explicit RoomCreationWidget(QWidget *parent = 0);
signals:
  void createRoom(QString name, QString pass, QString dmpass);
private slots:
  void createButtonPressed();
};

#endif // ROOMCREATIONWIDGET_H
