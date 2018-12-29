#include "aliassheetbinder.h"

const QString AliasSheetBinder::className = "binder";

const QList<AliasSheet *>& AliasSheetBinder::getSheets() const
{
  return sheets;
}

void AliasSheetBinder::makeValid()
{
  //remove invalid sheets
  for(int i=0;i<sheets.size();i++)
  {
    if(sheets[i]==nullptr)
    {
      sheets.removeAt(i);
      i--;
    }
  }
}

AliasSheetBinder::AliasSheetBinder()
{
  sheetName = QString();
}

AliasSheetBinder::AliasSheetBinder(const QList<AliasSheet *> &sheets, const QString &sheetName)
{
  this->sheetName = sheetName;
  for(int i=0;i<sheets.size();i++)
  {
    this->sheets.append(sheets[i]);
  }
  makeValid();
}

AliasSheetBinder::AliasSheetBinder(const QJsonObject &sheetObject)
{
  sheetName = QString();
  loadJson(sheetObject);
}

AliasSheetBinder::AliasSheetBinder(const AliasSheetBinder &other)
{
  sheetName = other.getSheetName();
  for(int i=0;i<other.sheets.size();i++)
  {
    sheets.append(other.sheets[i]->copy());
  }
}

AliasSheetBinder::~AliasSheetBinder()
{
  foreach(AliasSheet* sheet,sheets)
    delete sheet;
}

QJsonObject AliasSheetBinder::toJson() const
{
  QJsonObject result;
  result.insert("type",AliasSheetBinder::className);
  result.insert("name",sheetName);
  {
    QJsonArray sheetArray;
    for(int i=0;i<sheets.size();i++)
      sheetArray.append(sheets[i]->toJson());
    result.insert("sheets",sheetArray);
  }
  return result;
}

void AliasSheetBinder::loadJson(const QJsonObject &sheetObject)
{
  foreach(AliasSheet* sheet,sheets)
    delete sheet;
  sheets.clear();
  sheetName = sheetObject["name"].toString();
  //load sheets
  {
    QJsonArray sheetArray = sheetObject["sheets"].toArray();
    for(int i=0;i<sheetArray.size();i++)
    {
      sheets.append(AliasSheet::fromJson(sheetArray[i].toObject()));
    }
  }
  makeValid();
}

AliasSheet *AliasSheetBinder::copy() const
{
  AliasSheetBinder* binder = new AliasSheetBinder();
  binder->sheetName = sheetName;
  for(int i=0;i<sheets.size();i++)
    binder->sheets.append(sheets[i]->copy());
  return binder;
}

AliasSheet::SheetType AliasSheetBinder::getType() const
{
  return SHEET_BINDER;
}
