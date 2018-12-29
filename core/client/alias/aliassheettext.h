#ifndef ALIASSHEETTEXT_H
#define ALIASSHEETTEXT_H

#include <QString>
#include "aliassheet.h"

class AliasSheetText : public AliasSheet
{
  QString text;
public:
  const static QString className;
  AliasSheetText(const QString& sheetName, const QString& text=QString());
  AliasSheetText(const QJsonObject& sheetObject);
  virtual QJsonObject toJson() const;
  virtual void loadJson(const QJsonObject& sheetObject);
  virtual AliasSheet* copy() const;
  virtual SheetType getType() const;
  QString getText() const;
  void setText(const QString &value);
};

#endif // ALIASSHEETTEXT_H
