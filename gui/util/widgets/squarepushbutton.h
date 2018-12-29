#ifndef SQUAREPUSHBUTTON_H
#define SQUAREPUSHBUTTON_H

#include <QWidget>
#include <QPushButton>

class QString;
class QIcon;

class SquarePushButton : public QPushButton
{
  Q_OBJECT
public:
  explicit SquarePushButton(QWidget *parent = 0);
  explicit SquarePushButton(const QString& text, QWidget *parent = 0);
  explicit SquarePushButton(const QIcon& icon, const QString& text="", QWidget *parent = 0);
  explicit SquarePushButton(QPushButtonPrivate &dd, QWidget *parent);
protected:
  virtual int heightForWidth(int width) const;
  virtual void resizeEvent(QResizeEvent *);
signals:

public slots:
};

#endif // SQUAREPUSHBUTTON_H
