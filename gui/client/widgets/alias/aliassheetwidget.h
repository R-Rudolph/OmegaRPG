#ifndef ALIASSHEETWIDGET_H
#define ALIASSHEETWIDGET_H

#include <QWidget>
#include <QStackedLayout>
#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>
#include "core/client/alias/aliassheet.h"

class AliasSheetWidget : public QWidget
{
  Q_OBJECT
protected:
  QString sheetName;
  bool isEditWidget;
public:
  explicit AliasSheetWidget(bool edit, const QString& sheetName, QWidget *parent = 0);
  virtual AliasSheet* toAliasSheet() = 0;
  QString getSheetName() const;
  void setSheetName(const QString &value);
  static AliasSheetWidget* fromAliasSheet(const AliasSheet* sheet, bool editMode,  QWidget* parent=nullptr);
signals:
  void changed();
  void chat(const QString& text);
public slots:
  void exportSheet();
  virtual void setNightMode(bool value);
};

#endif // ALIASSHEETWIDGET_H
