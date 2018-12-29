#ifndef MINIATUREMENU_H
#define MINIATUREMENU_H

#include <QWidget>
#include <QVBoxLayout>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QInputDialog>
#include <QHeaderView>
#include <QMenu>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QFile>
#include <QSaveFile>
#include <QFileDialog>
#include <QLabel>

#include "core/multi/roomresources/mapresources/mapminiatureresource.h"
#include "dialogs/miniatureeditdialog.h"
#include "core/multi/util/global.h"

class MiniatureMenuWidget : public QWidget
{
  Q_OBJECT
  bool pendingSave;
  MapMiniatureResource miniature;
  QVBoxLayout* layout;
  QLabel* label;
  QTreeWidget* tree;

  QTreeWidgetItem* contextMenuItem;

  //persistence functions
  QJsonObject getJson(QTreeWidgetItem* item);
  QTreeWidgetItem* getItemFromJson(const QJsonObject& json);
  bool parseJson(const QJsonObject& json);
  void load();
  bool load(const QString& filename);
  void save();
  void save(const QString& filename);

  void setData(QTreeWidgetItem* item, const MapMiniatureResource& miniature, const QString& comment);
  QString getComment(QTreeWidgetItem* item) const;
  void setComment(QTreeWidgetItem* item, const QString& comment);
  MapMiniatureResource getResource(QTreeWidgetItem* item) const;
  void setResource(QTreeWidgetItem* item, const MapMiniatureResource& resource) const;
  //item functions
  QTreeWidgetItem* getMiniatureItem(MapMiniatureResource miniature, const QString& comment, const QJsonObject* fallback=nullptr);
  QTreeWidgetItem* getFolderItem(const QString& name);
  QTreeWidgetItem* getFolder(QTreeWidgetItem* item);
  bool isMiniature(const QTreeWidgetItem* item) const;
  void setIsMiniature(QTreeWidgetItem* item, bool value);
public:
  explicit MiniatureMenuWidget(QWidget *parent = 0);
  ~MiniatureMenuWidget();

signals:
  void newMiniature(QString name, QString addr, qreal x, qreal y, qreal size);
  void previewData(const MapMiniatureResource& miniature);

private slots:
  void itemSelected(QTreeWidgetItem* current,QTreeWidgetItem* previous);
  void addMiniAction(bool);
  void addFolderAction(bool);
  void addMini(QTreeWidgetItem* item);
  void addFolder(QTreeWidgetItem* item);
  void addToInitiativeList();
  void contextMenu(QPoint point);

  void doubleClick(QTreeWidgetItem*,int);
  void editMiniatureMenu();
  void editFolderMenu();
  void removeItem();
  void cloneItem();

  void exportMinis();
  void importMinis();
public slots:
  void hide();
  void miniatureImport(const MapMiniatureResource& miniature);
};

#endif // MINIATUREMENU_H
