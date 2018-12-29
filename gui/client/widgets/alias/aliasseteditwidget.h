#ifndef ALIASSETEDITWIDGET_H
#define ALIASSETEDITWIDGET_H

#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include "core/client/alias/aliasset.h"

class AliasSetEditWidget : public QWidget
{
  Q_OBJECT
  AliasSet* aliasSet;
  QFormLayout* layout;
  QLineEdit* nameEdit;
public:
  explicit AliasSetEditWidget(QWidget *parent = 0);

signals:
  void nameChanged(const QString& name);
  void changed();
public slots:
  void setAliasSet(AliasSet* aliasSet);
private slots:
  void nameEditChanged(const QString& name);
};

#endif // ALIASSETEDITWIDGET_H
