#ifndef MINIATURESIZESPINBOX_H
#define MINIATURESIZESPINBOX_H

#include <QDoubleSpinBox>

class MiniatureSizeSpinBox : public QDoubleSpinBox
{
  Q_OBJECT
public:
  explicit MiniatureSizeSpinBox(QWidget *parent = nullptr);
  virtual QString textFromValue(double val) const;
signals:

public slots:
    void stepBy(int steps);
};

#endif // MINIATURESIZESPINBOX_H
