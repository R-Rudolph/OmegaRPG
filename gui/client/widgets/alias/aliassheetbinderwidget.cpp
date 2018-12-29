#include "aliassheetbinderwidget.h"

#include "core/client/alias/aliassheetbinder.h"
#include "aliassheettextwidget.h"
#include "aliassheetnoteswidget.h"
#include "aliassheetdicewidget.h"
#include "aliassheetpluginwidget.h"
#include "gui/client/plugins/pluginlib.h"

QList<AliasSheet *> AliasSheetBinderWidget::getSheets()
{
  QList<AliasSheet *> result;
  for(int i=0;i<tabs->count();i++)
  {
    AliasSheetWidget* widget = static_cast<AliasSheetWidget*>(tabs->widget(i));
    AliasSheet* sheet = widget->toAliasSheet();
    sheet->setSheetName(tabs->tabText(i));
    result.append(sheet);
  }
  return result;
}

AliasSheetBinderWidget::AliasSheetBinderWidget(bool edit, const QString &sheetName, QWidget *parent) : AliasSheetWidget(edit, sheetName, parent)
{
  nightMode = false;
  new QStackedLayout(this);
  tabs = new QTabWidget(this);
  this->layout()->addWidget(tabs);
  if(isEditWidget)
  {
    tabs->setContextMenuPolicy(Qt::CustomContextMenu);
    tabs->setMovable(true);
    connect(tabs->tabBar(),&QTabBar::tabMoved,this,&AliasSheetBinderWidget::changed);
    connect(tabs,&QTabWidget::customContextMenuRequested,this,&AliasSheetBinderWidget::contextMenu);
    connect(tabs->tabBar(),&QTabBar::tabBarDoubleClicked,this,&AliasSheetBinderWidget::renameWidgetIndex);
  }
}

AliasSheetBinderWidget::AliasSheetBinderWidget(const AliasSheetBinder *sheet, bool edit, QWidget *parent)
  :AliasSheetBinderWidget(edit,sheet->getSheetName(),parent)
{
  foreach(const AliasSheet* subsheet, sheet->getSheets())
  {
    addSheetWidget(AliasSheetWidget::fromAliasSheet(subsheet,edit));
  }
}

AliasSheet *AliasSheetBinderWidget::toAliasSheet()
{
  return new AliasSheetBinder(getSheets(),sheetName);
}

void AliasSheetBinderWidget::addSheetWidget(AliasSheetWidget *sheet)
{
  if(sheet != nullptr)
  {
    tabs->addTab(sheet,sheet->getSheetName());
    sheet->setNightMode(nightMode);
    connect(sheet,&AliasSheetWidget::chat,this,&AliasSheetBinderWidget::chat);
    connect(sheet,&AliasSheetWidget::changed,this,&AliasSheetBinderWidget::changed);
  }
}

void AliasSheetBinderWidget::setNightMode(bool value)
{
   nightMode = value;
  for(int i=0;i<tabs->count();i++)
  {
    AliasSheetWidget* widget = static_cast<AliasSheetWidget*>(tabs->widget(i));
    widget->setNightMode(value);
  }
}

void AliasSheetBinderWidget::import()
{
  QString filepath = QFileDialog::getOpenFileName(this,"Import Sheet",QString(),"All Files (*)");
  if(!filepath.isEmpty())
  {
    QFile file(filepath);
    if(file.open(QIODevice::ReadOnly))
    {
      QJsonParseError error;
      QJsonDocument doc = QJsonDocument::fromJson(file.readAll(),&error);
      file.close();
      if(error.error == QJsonParseError::NoError)
      {
        AliasSheet* sheet = AliasSheet::fromJson(doc.object());
        if(sheet!=nullptr)
        {
          addSheetWidget(AliasSheetWidget::fromAliasSheet(sheet,isEditWidget,this));
          delete sheet;
          emit changed();
        }
        else
        {
          QMessageBox::critical(this,"Sheet Import","Import Error: Invalid sheet file.",QMessageBox::Ok,QMessageBox::Ok);
        }
      }
      else
      {
        QMessageBox::critical(this,"Sheet Import","Import Error: Malformed sheet file.",QMessageBox::Ok,QMessageBox::Ok);
      }
    }
    else
      QMessageBox::critical(this,"Sheet Import","Import Error: Couldn't open file.",QMessageBox::Ok,QMessageBox::Ok);
  }
}

void AliasSheetBinderWidget::contextMenu(const QPoint &pos)
{
  int index = tabs->tabBar()->tabAt(pos);
  QMenu menu;
  QMenu newSubmenu("New");
  newSubmenu.addAction("Binder",this,&AliasSheetBinderWidget::newBinder);
  newSubmenu.addAction("Notes Sheet",this,&AliasSheetBinderWidget::newNotesSheet);
  newSubmenu.addAction("Static HTML Sheet",this,&AliasSheetBinderWidget::newStaticSheet);
  newSubmenu.addAction("Dice Sheet",this,&AliasSheetBinderWidget::newDiceSheet);
  QMenu* pluginSubmenu = newSubmenu.addMenu("Plugin");
  getPluginSubmenu(pluginSubmenu);
  menu.addMenu(&newSubmenu);
  QAction* copyAction = menu.addAction("Copy",this,&AliasSheetBinderWidget::copyToClipboard);
  QAction* pasteAction = menu.addAction("Paste",this,&AliasSheetBinderWidget::pasteFromClipboard);
  menu.addSeparator();
  menu.addAction("Import",this,&AliasSheetBinderWidget::import);
  QAction* exportAction = menu.addAction("Export");
  menu.addSeparator();
  QAction* renameAction = menu.addAction("Rename",this,&AliasSheetBinderWidget::contextRenameSheet);
  QAction* removeAction = menu.addAction("Remove",this,&AliasSheetBinderWidget::contextRemoveSheet);
  {
    QString clip = QApplication::clipboard()->text();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(clip.toUtf8(),&error);
    if(error.error==QJsonParseError::NoError)
    {
      AliasSheet* sheet = AliasSheet::fromJson(doc.object());
      if(sheet==nullptr)
        pasteAction->setDisabled(true);
      delete sheet;
    }
    else
      pasteAction->setDisabled(true);
  }
  if(index!=-1)
  {
    currentContextMenuSheet = static_cast<AliasSheetWidget*>(tabs->widget(index));
    connect(exportAction,&QAction::triggered,currentContextMenuSheet,&AliasSheetWidget::exportSheet);
  }
  else
  {
    copyAction->setDisabled(true);
    exportAction->setDisabled(true);
    renameAction->setDisabled(true);
    removeAction->setDisabled(true);
  }
  menu.exec(tabs->mapToGlobal(pos));
  currentContextMenuSheet = nullptr;
}

void AliasSheetBinderWidget::newBinder()
{
  addSheetWidget(new AliasSheetBinderWidget(isEditWidget,"New Binder",this));
  emit changed();
}

void AliasSheetBinderWidget::newStaticSheet()
{
  addSheetWidget(new AliasSheetTextWidget(isEditWidget,"New Sheet",this));
  emit changed();
}

void AliasSheetBinderWidget::newNotesSheet()
{
  addSheetWidget(new AliasSheetNotesWidget(isEditWidget,"New Notes",this));
  emit changed();
}

void AliasSheetBinderWidget::newDiceSheet()
{
  addSheetWidget(new AliasSheetDiceWidget(isEditWidget,"New Dice",this));
  emit changed();
}

void AliasSheetBinderWidget::contextRemoveSheet()
{
  int answer = QMessageBox::question(this,
                                     "Remove Sheet",
                                     "Are you sure you want to delete the sheet '"+currentContextMenuSheet->getSheetName()+"'?",
                                     QMessageBox::Yes | QMessageBox::No,
                                     QMessageBox::No);
  if(answer==QMessageBox::Yes)
  {
    tabs->removeTab(tabs->indexOf(currentContextMenuSheet));
    emit changed();
  }
}

void AliasSheetBinderWidget::contextRenameSheet()
{
  bool ok;
  QString name = QInputDialog::getText(this,
                                       "Sheet Name", "New name for Sheet '"+currentContextMenuSheet->getSheetName()+"'.",
                                       QLineEdit::Normal,
                                       currentContextMenuSheet->getSheetName(),
                                       &ok);
  if(ok)
  {
    tabs->setTabText(tabs->indexOf(currentContextMenuSheet),name);
    currentContextMenuSheet->setSheetName(name);
    emit changed();
  }
}

void AliasSheetBinderWidget::copyToClipboard()
{
  AliasSheet* sheet = currentContextMenuSheet->toAliasSheet();
  QJsonDocument doc(sheet->toJson());
  QApplication::clipboard()->setText(QString::fromUtf8(doc.toJson()));
}

void AliasSheetBinderWidget::pasteFromClipboard()
{
  QString clip = QApplication::clipboard()->text();
  QJsonParseError error;
  QJsonDocument doc = QJsonDocument::fromJson(clip.toUtf8(),&error);
  if(error.error==QJsonParseError::NoError)
  {
    AliasSheet* sheet = AliasSheet::fromJson(doc.object());
    if(sheet!=nullptr)
    {
      addSheetWidget(AliasSheetWidget::fromAliasSheet(sheet,isEditWidget,this));
      delete sheet;
      emit changed();
    }
  }
}

void AliasSheetBinderWidget::renameWidgetIndex(int index)
{
  if(index>=0)
  {
    currentContextMenuSheet = static_cast<AliasSheetWidget*>(tabs->widget(index));
    contextRenameSheet();
  }
}

void AliasSheetBinderWidget::newPlugin()
{
  QAction* action = dynamic_cast<QAction*>(sender());
  QString plugin = action->data().toString();
  AliasSheetPlugin sheet("New Plugin",plugin);
  addSheetWidget(AliasSheetWidget::fromAliasSheet(&sheet,isEditWidget,this));
  emit changed();
}

void AliasSheetBinderWidget::getPluginSubmenu(QMenu* menu)
{
  QMap<QString,PluginInfo> plugins = PluginLib::get()->plugins();
  if(!plugins.isEmpty())
  {
    for(auto it = plugins.begin();it!=plugins.end();it++)
    {
      if(it.value().isValid())
      {
        QAction* action = menu->addAction(it.value().name().replace("&","&&"),this,&AliasSheetBinderWidget::newPlugin);
        action->setData(it.key());
      }
    }
  }
  if(menu->actions().isEmpty())
    menu->setDisabled(true);
}
