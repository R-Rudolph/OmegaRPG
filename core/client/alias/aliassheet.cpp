#include "aliassheet.h"

#include "aliassheettext.h"
#include "core/client/alias/aliassheetbinder.h"
#include "aliassheetnotes.h"
#include "aliassheetdice.h"
#include "aliassheetplugin.h"

QString AliasSheet::getSheetName() const
{
  return sheetName;
}

void AliasSheet::setSheetName(const QString &value)
{
  sheetName = value;
}

AliasSheet::~AliasSheet()
{

}

AliasSheet *AliasSheet::fromJson(const QJsonObject &sheetObject)
{
  QString type = sheetObject["type"].toString();
  if(type==AliasSheetText::className)
    return new AliasSheetText(sheetObject);
  else if(type==AliasSheetBinder::className)
    return new AliasSheetBinder(sheetObject);
  else if(type == AliasSheetNotes::className)
    return new AliasSheetNotes(sheetObject);
  else if(type == AliasSheetDice::className)
    return new AliasSheetDice(sheetObject);
  else if(type == AliasSheetPlugin::className)
    return new AliasSheetPlugin(sheetObject);
  else
    return nullptr;
}
