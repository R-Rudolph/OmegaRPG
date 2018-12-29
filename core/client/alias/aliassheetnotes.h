#ifndef ALIASSHEETNOTES_H
#define ALIASSHEETNOTES_H

#include "aliassheet.h"
#include <QString>

class AliasSheetNotes : public AliasSheet
{
  QString text;
public:
  const static QString className;
  AliasSheetNotes();
  AliasSheetNotes(const QJsonObject& sheetObject);
  AliasSheetNotes(const AliasSheetNotes& other);
  AliasSheetNotes(const QString& text);
  virtual QJsonObject toJson() const;
  virtual void loadJson(const QJsonObject& sheetObject);
  virtual AliasSheet* copy() const;
  QString getText() const;
  void setText(const QString &value);
  virtual SheetType getType() const;
};

#endif // ALIASSHEETNOTES_H
