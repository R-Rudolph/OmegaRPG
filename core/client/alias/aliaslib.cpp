#include "aliaslib.h"
#include "util/qtio.h"

AliasLib* AliasLib::staticLib = nullptr;

bool AliasLib::load(const QString &filename)
{
  QFile file(filename);
  if(!file.open(QIODevice::ReadOnly))
  {
    fillDummyData();
    errorMsg = "Could not open file.";
    if(file.exists())
      lastLoadError = LOAD_OPEN_ERROR;
    else
      lastLoadError = LOAD_EXISTS_ERROR;
    return false;
  }
  QByteArray data = file.readAll();
  QJsonParseError parseError;
  QJsonDocument doc = QJsonDocument::fromJson(data,&parseError);
  if(parseError.error!=QJsonParseError::NoError)
  {
    errorMsg = parseError.errorString();
    lastLoadError = LOAD_JSON_ERROR;
    lastParseError = parseError;
    return false;
  }
  QJsonArray arr = doc.array();
  sets.clear();
  for(int i=0;i<arr.size();i++)
  {
    if(!arr.at(i).isObject())
      continue;
    AliasSet newSet(arr.at(i).toObject());
    sets.push_back(newSet);
  }
  lastLoadError = LOAD_OK;
  return true;
}

void AliasLib::save(const QString &filename)
{
  QSaveFile file(filename);
  if(!file.open(QIODevice::WriteOnly))
  {
    QtIO::err << "Error: Could not open \"" << filename << "\"." << endl;
    emit saveFailed();
    return;
  }
  QJsonArray arr = toJson();
  QJsonDocument doc(arr);
  QByteArray data = doc.toJson();
  file.write(data);
  file.commit();
}

AliasLib::AliasLib(QObject *parent) : QObject(parent)
{
  lastLoadError = LOAD_OK;
  currentAlias = nullptr;
  connect(orpg::Settings::get(),&orpg::Settings::aliasChanged,this,&AliasLib::mainAliasChanged);
}

AliasLib::AliasLib(const QString &filename, QObject *parent) : AliasLib(parent)
{
  load(filename);
}

QJsonArray AliasLib::toJson() const
{
  QJsonArray arr;
  for(int i=0;i<sets.size();i++)
    arr.push_back(sets.at(i).toJson());
  return arr;
}

void AliasLib::fillDummyData()
{
  AliasSet set1;
  set1.name = "Set 1";
  set1.set.push_back(Alias("Alias 1","#FF0000"));
  set1.set.push_back(Alias("Alias 2","#000000"));
  AliasSet set2;
  set2.selected = true;
  set2.name = "Set 2";
  set2.set.push_back(Alias("Alias 3","#FF0000"));
  set2.set.push_back(Alias("Alias 4","#000000"));
  sets.push_back(set1);
  sets.push_back(set2);
}

void AliasLib::updateSelectedAliases(Alias alias)
{
  QList<const Alias*> newSelectedAliases;
  newSelectedAliases.append(&(orpg::Settings::get()->player));
  foreach(const AliasSet& set,sets)
  {
    newSelectedAliases.append(set.getSelectedAliases());
  }

  if(newSelectedAliases!=selectedAliases)
  {
    selectedAliases = newSelectedAliases;
    emit selectedAliasesChanged();
  }
  if(currentAlias!=nullptr)
  {
    for(int i=0;i<selectedAliases.size();i++)
    {
      if((Alias)*(selectedAliases[i]) == alias)
      {
        setCurrentAliasIndex(i);
        return;
      }
    }
  }
  setCurrentAliasIndex(0);
}

void AliasLib::updateSelectedAliases()
{
  Alias alias;
  if(currentAlias!=nullptr)
    alias = *currentAlias;
  updateSelectedAliases(alias);
}

QString AliasLib::getLoadErrorString() const
{
  switch(lastLoadError)
  {
    case LOAD_OPEN_ERROR:
      return QString("Could not read alias file. Permission denied.");
    case LOAD_JSON_ERROR:
      return QString("Json Error for alias file: "+lastParseError.errorString());
    case LOAD_EXISTS_ERROR:
      return QString("Alias Library file not found. This is normal if you didn't create an Alias Library yet.");
    default:
      return QString();
  }
}

AliasLib::LoadError AliasLib::getLoadError() const
{
  return lastLoadError;
}

AliasLib *AliasLib::get()
{
  if(AliasLib::staticLib==nullptr)
  {
    AliasLib::staticLib = new AliasLib();
    AliasLib::staticLib->load(Global::aliasLibFilepath());
    AliasLib::staticLib->updateSelectedAliases();
  }
  return AliasLib::staticLib;
}

void AliasLib::set(const AliasLib &lib)
{
  set(&lib);
}

void AliasLib::set(const AliasLib *lib)
{
  Alias alias;
  if(currentAlias!=nullptr)
      alias = *currentAlias;
  this->sets = lib->sets;
  updateSelectedAliases(alias);
  if(this == AliasLib::staticLib)
    this->save(Global::aliasLibFilepath());
}

void AliasLib::import(const AliasLib &lib)
{
  for(int i=0;i<lib.sets.size();i++)
  {
    sets.push_back(lib.sets[i]);
  }
}

bool AliasLib::import(const QString &filename)
{
  AliasLib lib;
  if(lib.load(filename))
  {
    import(lib);
    return true;
  }
  return false;
}

bool AliasLib::importOpenRPG(const QString &filename, const QString &setName)
{
  QFile file(filename);
  QString name = setName;
  if(name.isEmpty())
    name = QFileInfo(file).fileName();
  if(file.open(QIODevice::ReadOnly))
  {
    QDomDocument doc;
    int row,col;
    if(doc.setContent(&file,&errorMsg,&row,&col))
    {
      file.close();
      QDomElement aliaslibElem = doc.documentElement();
      if(aliaslibElem.tagName()!="aliaslib")
      {
        errorMsg = "File does not seem to be in OpenRPG Alias Library format.";
        return false;
      }
      QDomNodeList list = aliaslibElem.elementsByTagName("alias");
      AliasSet set;
      set.name = name;
      for(int i=0;i<list.size();i++)
      {
        Alias alias;
        QDomElement elem = list.item(i).toElement();
        alias.setName(elem.attribute("name","No Name"));
        alias.setColor(elem.attribute("color","#000080"));
        set.set.push_back(alias);
      }
      sets.push_back(set);
      errorMsg = QString();
      return true;
    }
    else
    {
      file.close();
      QtIO::err << "parsing error: "<< errorMsg << "(" << row << "," << col << ")" << endl;
      return false;
    }
  }
  errorMsg = "Could not open file.";
  return false;
}

QString AliasLib::error()
{
  return errorMsg;
}

int AliasLib::getCurrentAliasIndex()
{
  int index = selectedAliases.indexOf(currentAlias);
  if(index==-1)
    index = 0;
  return index;
}

const Alias *AliasLib::getCurrentAlias()
{
  return currentAlias;
}

Alias *AliasLib::getCurrentAliasEditable()
{
  return (Alias*)currentAlias;
}

const QList<const Alias *> &AliasLib::getSelectedAliases()
{
  return selectedAliases;
}

AliasSet &AliasLib::addSet(const QString &name)
{
  sets.push_back(AliasSet(name));
  return sets[sets.size()-1];
}

void AliasLib::setCurrentAliasIndex(int index)
{
  if(index < selectedAliases.size() && index>=0)
  {
    currentAlias = selectedAliases[index];
    emit currentAliasChanged(currentAlias);
    emit currentAliasIndexChanged(index);
  }
}

void AliasLib::clear()
{
  sets.clear();
  updateSelectedAliases();
}

void AliasLib::notifyChange(Alias *alias)
{
  emit libraryChanged();
  if(alias == &orpg::Settings::get()->player)
  {
    orpg::Settings::get()->notifyAliasChange();
  }
  else if(this == AliasLib::staticLib)
    this->save(Global::aliasLibFilepath());
  if(alias==currentAlias)
  {
    emit currentAliasChanged(currentAlias);
    emit currentAliasIndexChanged(selectedAliases.indexOf(currentAlias));
  }
}

void AliasLib::mainAliasChanged()
{
  emit selectedAliasesChanged();
  if(currentAlias == &orpg::Settings::get()->player)
  {
    emit currentAliasChanged(currentAlias);
    emit currentAliasIndexChanged(selectedAliases.indexOf(currentAlias));
  }
}
