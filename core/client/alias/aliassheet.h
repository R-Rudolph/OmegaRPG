#ifndef ALIASSHEET_H
#define ALIASSHEET_H

#include <QWidget>
#include <QJsonObject>

class AliasSheetWidget;

class AliasSheet
{
public:
  enum SheetType
  {
    SHEET_INVALID,
    SHEET_BINDER,
    SHEET_DICE,
    SHEET_NOTES,
    SHEET_TEXT,
    SHEET_PLUGIN
  };
protected:
  QString sheetName;
public:
  virtual ~AliasSheet();
  virtual QJsonObject toJson() const = 0;
  virtual void loadJson(const QJsonObject& sheetObject) = 0;
  virtual AliasSheet* copy() const = 0;
  static AliasSheet* fromJson(const QJsonObject& sheetObject);
  QString getSheetName() const;
  void setSheetName(const QString &value);
  virtual SheetType getType() const =0;
};

#endif // ALIASSHEET_H
