#include "aliassheetwidget.h"

#include "aliassheetbinderwidget.h"
#include "aliassheetdicewidget.h"
#include "aliassheettextwidget.h"
#include "aliassheetnoteswidget.h"
#include "aliassheetpluginwidget.h"

#include "core/client/alias/aliassheetbinder.h"
#include "core/client/alias/aliassheetdice.h"
#include "core/client/alias/aliassheettext.h"
#include "core/client/alias/aliassheetnotes.h"
#include "core/client/alias/aliassheetplugin.h"

QString AliasSheetWidget::getSheetName() const
{
  return sheetName;
}

void AliasSheetWidget::setSheetName(const QString &value)
{
  sheetName = value;
}

AliasSheetWidget *AliasSheetWidget::fromAliasSheet(const AliasSheet *sheet, bool editMode, QWidget* parent)
{
  AliasSheetWidget* result = nullptr;
  switch(sheet->getType())
  {
    case AliasSheet::SHEET_BINDER:
      result = new AliasSheetBinderWidget(static_cast<const AliasSheetBinder*>(sheet),editMode,parent);
      break;
    case AliasSheet::SHEET_DICE:
      result = new AliasSheetDiceWidget(static_cast<const AliasSheetDice*>(sheet),editMode,parent);
      break;
    case AliasSheet::SHEET_NOTES:
      result = new AliasSheetNotesWidget(static_cast<const AliasSheetNotes*>(sheet),editMode,parent);
      break;
    case AliasSheet::SHEET_TEXT:
      result = new AliasSheetTextWidget(static_cast<const AliasSheetText*>(sheet),editMode,parent);
      break;
    case AliasSheet::SHEET_PLUGIN:
      result = new AliasSheetPluginWidget(static_cast<const AliasSheetPlugin*>(sheet),editMode,parent);
      break;
    case AliasSheet::SHEET_INVALID:
      break;
  }
  return result;
}

void AliasSheetWidget::exportSheet()
{
  QString filepath = QFileDialog::getSaveFileName(this, "Export Alias Sheet", QString(), "All Files (*)");
  if(!filepath.isEmpty())
  {
    QFile file(filepath);
    if(file.open(QIODevice::WriteOnly))
    {
      AliasSheet* sheet = toAliasSheet();
      QJsonDocument doc(sheet->toJson());
      file.write(doc.toJson());
      delete sheet;
      file.close();
    }
    else
    {
      QMessageBox::critical(this,"Export Error","Export Error: Couldn't write file.",QMessageBox::Ok,QMessageBox::Ok);
    }
  }
}

void AliasSheetWidget::setNightMode(bool)
{

}

AliasSheetWidget::AliasSheetWidget(bool edit, const QString &sheetName, QWidget *parent) : QWidget(parent)
{
  this->sheetName = sheetName;
  isEditWidget = edit;
}
