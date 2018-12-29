#ifndef NIGHTMODETEXTEDIT_H
#define NIGHTMODETEXTEDIT_H

#include <QWidget>
#include <QTextEdit>

class NightModeTextEdit : public QTextEdit
{
  Q_OBJECT
public:
  explicit NightModeTextEdit(QWidget *parent = 0);
  explicit NightModeTextEdit(const QString& text, QWidget *parent = 0);

signals:

public slots:
private slots:
  void setNightMode(bool value);
};

#endif // NIGHTMODETEXTEDIT_H
