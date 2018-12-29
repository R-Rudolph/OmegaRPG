#ifndef PLUGININFOWIDGET_H
#define PLUGININFOWIDGET_H

#include <QObject>
#include <QWidget>
#include <QFormLayout>
#include <QLabel>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QStackedLayout>
#include "gui/client/plugins/plugininfo.h"

class PluginInfoWidget : public QWidget
{
  Q_OBJECT
  QStackedLayout* layout_;
  QVBoxLayout* errorLayout_;
  QFormLayout* mainLayout_;
  QLabel* nameLabel_;
  QTextBrowser* descriptionText_;
  QTextBrowser* errorsText_;
public:
  explicit PluginInfoWidget(QWidget *parent = nullptr);
  void setInfo(const PluginInfo& info);
signals:

public slots:
};

#endif // PLUGININFOWIDGET_H
