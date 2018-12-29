#ifndef ALIASSHEETEDITWIDGET_H
#define ALIASSHEETEDITWIDGET_H

#include <QWidget>
#include <QStackedLayout>
#include <QTimer>
#include "core/client/alias/aliaslib.h"
#include "aliassheetbinderwidget.h"
#include "widgets/coloroverlaywrapperwidget.h"

class AliasSheetEditWidget : public QWidget
{
  Q_OBJECT
  AliasSheetBinderWidget* binder;
  Alias* currentAlias;
  bool nightMode;
  void updateNightMode();
public:
  explicit AliasSheetEditWidget(QWidget *parent = 0);

signals:
  void editedAlias(Alias* alias);
  void somethingChanged();
private slots:
  void changed();
public slots:
  void setAlias(Alias* alias);
  void setNightMode(bool value);
};

#endif // ALIASSHEETEDITWIDGET_H
