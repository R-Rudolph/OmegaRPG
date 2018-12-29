#ifndef GLOBALGUI_H
#define GLOBALGUI_H

#include <QIcon>
#include <QStyle>
#include "core/multi/util/global.h"

class GlobalGUI
{
  static QIcon iconByName(const QString& name);
  static QIcon standardIcon(QStyle::StandardPixmap pixmap);
  static QStyle* style;
public:
  static void initIcons(QApplication& application);
  static QIcon getOmegaRPGIcon();
  //x11 icon theme functions
  static QIcon freedesktopActive();
  static QIcon freedesktopAdd();
  static QIcon freedesktopNewFolder();
  static QIcon freedesktopRemove();
  static QIcon freedesktopCopy();
  static QIcon freedesktopCut();
  static QIcon freedesktopDelete();
  static QIcon freedesktopPaste();
  static QIcon freedesktopTextEditor();
  static QIcon freedesktopSortAscending();
  static QIcon freedesktopZoomFitBest();
  static QIcon freedesktopSave();
  static QIcon freedesktopLoad();
  static QIcon freedesktopAbout();
  static QIcon freedesktopSystem();
  static QIcon freedesktopExit();
  static QIcon freedesktopSecurityLow();
  static QIcon freedesktopSecurityMedium();
  static QIcon freedesktopSecurityHigh();
  static QIcon serverIcon();
  static QIcon freedesktopFolder();
  static QIcon freedesktopGoNext();
  static QIcon refreshIcon();
};

#endif // GLOBALGUI_H
