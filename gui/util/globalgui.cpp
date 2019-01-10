#include "globalgui.h"
#include "util/qtio.h"
#include <QDir>
#include <QApplication>

QStyle* GlobalGUI::style = nullptr;

QIcon GlobalGUI::iconByName(const QString &name)
{
  QIcon icon = QIcon::fromTheme(name);
  if(icon.isNull())
  {
    icon = customIcon(name);
  }
  return icon;
}

QIcon GlobalGUI::standardIcon(QStyle::StandardPixmap pixmap)
{
  if(style)
  {
    QIcon icon = style->standardIcon(pixmap);
    return icon;
  }
  else
  {
    QtIO::err << "Error: Could not standard icon; style not set" << endl;
    return QIcon();
  }
}

QIcon GlobalGUI::customIcon(const QString& name)
{
  return QIcon(":icons/misc/"+name+".png");
}

void GlobalGUI::initIcons(QApplication& application)
{
  #ifdef _WIN32
  QIcon::setThemeSearchPaths({":/icons"});
  QIcon::setThemeName("oxygen");
  #endif
  style = application.style();
}

QIcon GlobalGUI::getOmegaRPGIcon()
{
  return QIcon(":icons/omegarpg.png");
}

QIcon GlobalGUI::iconActive()
{
  return standardIcon(QStyle::SP_DialogApplyButton);
}

QIcon GlobalGUI::iconAdd()
{
  return iconByName("list-add");
}

QIcon GlobalGUI::freedesktopNewFolder()
{
  return standardIcon(QStyle::SP_FileDialogNewFolder);
}

QIcon GlobalGUI::freedesktopRemove()
{
  return GlobalGUI::iconByName("list-remove");
}

QIcon GlobalGUI::freedesktopCopy()
{
  return GlobalGUI::iconByName("edit-copy");
}

QIcon GlobalGUI::freedesktopCut()
{
  return GlobalGUI::iconByName("edit-cut");
}

QIcon GlobalGUI::freedesktopDelete()
{
  return GlobalGUI::iconByName("edit-delete");
}

QIcon GlobalGUI::freedesktopPaste()
{
  return GlobalGUI::iconByName("edit-paste");
}

QIcon GlobalGUI::freedesktopTextEditor()
{
  return GlobalGUI::iconByName("edit");
}

QIcon GlobalGUI::freedesktopSortAscending()
{
  return GlobalGUI::iconByName("view-sort-ascending");
}

QIcon GlobalGUI::freedesktopZoomFitBest()
{
  return GlobalGUI::iconByName("zoom");
}

QIcon GlobalGUI::freedesktopSave()
{
  return GlobalGUI::iconByName("document-save");
}

QIcon GlobalGUI::freedesktopLoad()
{
  return GlobalGUI::iconByName("document-open");
}

QIcon GlobalGUI::freedesktopAbout()
{
  return GlobalGUI::iconByName("help-about");
}

QIcon GlobalGUI::freedesktopSystem()
{
  return GlobalGUI::iconByName("document-properties");
}

QIcon GlobalGUI::freedesktopExit()
{
  return GlobalGUI::iconByName("application-exit");
}

QIcon GlobalGUI::freedesktopSecurityLow()
{
  return GlobalGUI::iconByName("security-low");
}

QIcon GlobalGUI::freedesktopSecurityMedium()
{
  return GlobalGUI::iconByName("security-medium");
}

QIcon GlobalGUI::freedesktopSecurityHigh()
{
  return GlobalGUI::iconByName("security-high");
}

QIcon GlobalGUI::serverIcon()
{
  return standardIcon(QStyle::SP_DriveNetIcon);
}

QIcon GlobalGUI::freedesktopFolder()
{
  return GlobalGUI::iconByName("folder");
}

QIcon GlobalGUI::freedesktopGoNext()
{
  return GlobalGUI::iconByName("go-next");
}

QIcon GlobalGUI::refreshIcon()
{
  return standardIcon(QStyle::SP_BrowserReload);
}
