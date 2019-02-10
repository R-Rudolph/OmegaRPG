#ifndef ALIASSHEETMAINWIDGET_H
#define ALIASSHEETMAINWIDGET_H

#include <QWidget>
#include <QStackedLayout>
#include <QTimer>
#include "core/client/alias/aliaslib.h"
#include "aliassheetbinderwidget.h"

class AliasSheetReadWidget : public QWidget
{
  Q_OBJECT
  QTimer* updateTimer;
  AliasSheetBinderWidget* binder;
  const Alias* currentAlias;
  bool ignoreChange;
  bool nightMode;
  void updateNightMode();
public:
  explicit AliasSheetReadWidget(QWidget *parent = 0);

signals:
  void editedAlias(Alias* alias);
  void chat(const QString& text);
  void error(const QString& text);
public slots:
  void setNightMode(bool value);
private slots:
  void currentChanged(const Alias *alias);
  void changed();
  void updateAlias();
};

#endif // ALIASSHEETMAINWIDGET_H
