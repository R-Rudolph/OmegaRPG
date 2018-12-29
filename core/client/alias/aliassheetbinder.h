#ifndef ALIASSHEETBINDER_H
#define ALIASSHEETBINDER_H

#include <QString>
#include <QList>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include "aliassheet.h"

class AliasSheetBinder : public AliasSheet
{
  QList<AliasSheet*> sheets;
  void makeValid();
public:
  const static QString className;
  AliasSheetBinder();
  AliasSheetBinder(const QList<AliasSheet*>& sheets, const QString& sheetName);
  AliasSheetBinder(const QJsonObject& sheetObject);
  AliasSheetBinder(const AliasSheetBinder& other);
  ~AliasSheetBinder();
  virtual QJsonObject toJson() const;
  virtual void loadJson(const QJsonObject& sheetObject);
  virtual AliasSheet* copy() const;
  virtual SheetType getType() const;
  const QList<AliasSheet *>& getSheets() const;
};

#endif // ALIASSHEETBINDER_H
