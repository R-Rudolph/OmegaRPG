#ifndef ALIASEDITWIDGET_H
#define ALIASEDITWIDGET_H

#include <QWidget>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include "core/client/alias/alias.h"
#include "gui/client/widgets/alias/aliassheeteditwidget.h"
#include "gui/util/widgets/colorselectbutton.h"

class AliasEditWidget : public QWidget
{
  Q_OBJECT
  Alias* alias;
  QFormLayout* layout;
  QLineEdit* nameEdit;
  ColorSelectButton* colorButton;
  QLabel* colorLabel;
  AliasSheetEditWidget* aliasSheet;
public:
  explicit AliasEditWidget(QWidget *parent = 0);

signals:
  void nameChanged(const QString& name);
  void changed();
public slots:
  void setAlias(Alias* alias);
  void setNightMode(bool value);
private slots:
  void nameEditChanged(const QString& name);
  void colorChanged(const QColor& color);
  void colorChangedTemporary(const QColor& color);
};

#endif // ALIASEDITWIDGET_H
