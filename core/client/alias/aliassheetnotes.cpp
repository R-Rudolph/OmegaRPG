#include "aliassheetnotes.h"

const QString AliasSheetNotes::className = "notes";

AliasSheetNotes::AliasSheetNotes()
{

}

AliasSheetNotes::AliasSheetNotes(const QJsonObject &sheetObject)
{
  loadJson(sheetObject);
}

AliasSheetNotes::AliasSheetNotes(const AliasSheetNotes &other)
{
  text = other.getText();
  sheetName = other.getSheetName();
}

AliasSheetNotes::AliasSheetNotes(const QString &text)
{
  this->text = text;
}

QJsonObject AliasSheetNotes::toJson() const
{
  QJsonObject result;
  result.insert("type",AliasSheetNotes::className);
  result.insert("name",sheetName);
  result.insert("text",text);
  return result;
}

void AliasSheetNotes::loadJson(const QJsonObject &sheetObject)
{
  text = sheetObject["text"].toString();
  sheetName = sheetObject["name"].toString();
}

AliasSheet *AliasSheetNotes::copy() const
{
  AliasSheet* sheet = new AliasSheetNotes(text);
  sheet->setSheetName(sheetName);
  return sheet;
}

QString AliasSheetNotes::getText() const
{
  return text;
}

void AliasSheetNotes::setText(const QString &value)
{
  text = value;
}

AliasSheet::SheetType AliasSheetNotes::getType() const
{
  return SHEET_NOTES;
}
