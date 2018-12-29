#ifndef INITIATIVETREEWIDGET_H
#define INITIATIVETREEWIDGET_H

#include <QWidget>
#include <QTreeWidget>

class InitiativeTreeWidget : public QTreeWidget
{
  Q_OBJECT
public:
  explicit InitiativeTreeWidget(QWidget *parent = nullptr);
protected:
  void dropEvent(QDropEvent *event);
signals:
  void itemDropped(int fromPosition, int toPosition);
public slots:
};

#endif // INITIATIVETREEWIDGET_H
