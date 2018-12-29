#ifndef MINIATUREEDITDIALOG_H
#define MINIATUREEDITDIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QMessageBox>
#include <QSpinBox>
#include <QDoubleSpinBox>

#include "core/multi/roomresources/mapresources/mapminiatureresource.h"
#include "gui/util/globalgui.h"

#include "widgets/colorselectbutton.h"
#include "dialogs/nightmodedialog.h"
#include "widgets/imageurlselection.h"

class MiniatureSizeSpinBox;

class MiniatureEditDialog : public NightModeDialog
{
  MapMiniatureResource miniature;

  QGridLayout* layout;
  //QGridLayout* layout;
  QLineEdit* nameEdit;
  QComboBox* imageSourceBox;
  ColorSelectButton* colorButton;
  ImageUrlSelection* urlEdit;
  MiniatureSizeSpinBox* sizeBox;
  QComboBox* layerBox;
  QComboBox* visibilityBox;
  QComboBox* displayBox;
  QLineEdit* commentEdit;

  QComboBox* rotationBox;
  QSpinBox* rotationSpinBox;
  QComboBox* directionBox;
  QSpinBox* directionSpinBox;


  QHBoxLayout* buttonLayout;
  QPushButton* okButton;
  QPushButton* cancelButton;

  void setUrlMode(bool url);
  bool getUrlMode() const;

  QMap<QWidget*,QLabel*> labelMap;

  void addRow(const QString& label, QWidget* widget);
  void addRow(QWidget* widget);
  void addRow(QLayout* layout);
  void setWidgetHidden(QWidget* widget, bool value);

  void setRotation(qreal deg);
  void setDirection(qreal deg);

public:
  MiniatureEditDialog(QWidget* parent = 0);

  QString getName() const;
  void setName(const QString &value);
  QString getUrl() const;
  void setUrl(const QString &value);
  qreal getSize() const;
  void setSize(const qreal &value);
  int getId() const;
  void setId(int value);
  void setComment(const QString& value);
  QString getComment() const;
  void setLayer(MapMiniatureResource::Layer value);
  MapMiniatureResource::Layer getLayer() const;
  void setDisplay(MapMiniatureResource::Display value);
  MapMiniatureResource::Display getDisplay() const;
  void setVisibility(MapMiniatureResource::Visibility value);
  MapMiniatureResource::Visibility getVisibility() const;
  MapMiniatureResource getMiniature();
  void setMiniature(const MapMiniatureResource& mini);
  void setMiniatureMenuMode(bool value);
private slots:
  void okSlot();
  void sourceChanged(int index);
  void rotationBoxChanged(int index);
  void directionBoxChanged(int index);
};

#endif // MINIATUREEDITDIALOG_H
