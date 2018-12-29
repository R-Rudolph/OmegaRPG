#ifndef ALIASLIB_H
#define ALIASLIB_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QFile>
#include <QSaveFile>
#include <QJsonDocument>

#include <QDomDocument>
#include <QFileInfo>
#include <QList>
#include "aliasset.h"
#include "core/multi/util/global.h"
#include "core/client/settings.h"

class AliasLib : public QObject
{
  Q_OBJECT
public:
  enum LoadError
  {
    LOAD_OK,
    LOAD_OPEN_ERROR,
    LOAD_EXISTS_ERROR,
    LOAD_JSON_ERROR
  };

private:
  static AliasLib* staticLib;
  QJsonArray toJson() const;
  void fillDummyData();
  QString errorMsg;
  const Alias* currentAlias;
  QList<const Alias*> selectedAliases;
  void updateSelectedAliases(Alias alias);
  void updateSelectedAliases();

  LoadError lastLoadError;
  QJsonParseError lastParseError;
public:
  QString getLoadErrorString() const;
  static AliasLib* get();
  QVector<AliasSet> sets;

  explicit AliasLib(QObject* parent=0);
  explicit AliasLib(const QString& filename, QObject* parent=0);
  bool load(const QString& filename);
  void save(const QString& filename);

  void import(const AliasLib& lib);
  bool import(const QString& filename);
  bool importOpenRPG(const QString& filename, const QString& setName="");

  QString error();

  int getCurrentAliasIndex();
  const Alias* getCurrentAlias();
  Alias* getCurrentAliasEditable();
  const QList<const Alias*>& getSelectedAliases();
  AliasSet& addSet(const QString& name);
  LoadError getLoadError() const;

signals:
  void libraryChanged();
  void selectedAliasesChanged();
  void currentAliasChanged(const Alias* alias);
  void currentAliasIndexChanged(int index);
  void saveFailed();
public slots:
  void set(const AliasLib& lib);
  void set(const AliasLib* lib);
  void setCurrentAliasIndex(int index);
  void clear();
  void notifyChange(Alias* alias);
private slots:
  void mainAliasChanged();
};

#endif // ALIASLIB_H
