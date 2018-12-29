#include "aliassheettext.h"

const QString AliasSheetText::className = "text";

QString AliasSheetText::getText() const
{
  return text;
}

void AliasSheetText::setText(const QString &value)
{
  text = value;
}

AliasSheetText::AliasSheetText(const QString &sheetName, const QString &text)
{
  this->sheetName = sheetName;
  this->text = text;
}

AliasSheetText::AliasSheetText(const QJsonObject &sheetObject)
{
  loadJson(sheetObject);
}

QJsonObject AliasSheetText::toJson() const
{
  QJsonObject result;
  result.insert("type",AliasSheetText::className);
  result.insert("text",text);
  result.insert("name",sheetName);
  return result;
}

void AliasSheetText::loadJson(const QJsonObject &sheetObject)
{
  text = sheetObject["text"].toString();
  sheetName = sheetObject["name"].toString();
}

AliasSheet *AliasSheetText::copy() const
{
  return new AliasSheetText(sheetName, text);
}

AliasSheet::SheetType AliasSheetText::getType() const
{
  return SHEET_TEXT;
}
