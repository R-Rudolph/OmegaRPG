#include "roompropertiesutility.h"
#include "message/message.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

const QString RoomPropertiesUtility::variableString = "variables";
const QString RoomPropertiesUtility::resourceString = "resources";

bool RoomPropertiesUtility::saveRoomProperties(RoomVariableContainerCollection *variableCollection, ResourceContainerCollection *resourceCollection, const QString &filepath)
{
  QFile file(filepath);
  if(file.open(QIODevice::WriteOnly))
  {
    QJsonObject object;
    {
      QJsonArray variableArray;
      foreach(orpg::Message msg, variableCollection->getAllVariableMessages())
        variableArray.append(msg.toJson());
      object.insert(variableString,variableArray);
    }
    {
      QJsonArray resourceArray;
      foreach(orpg::Message msg, resourceCollection->getAllResourceMessages())
        resourceArray.append(msg.toJson());
      object.insert(resourceString,resourceArray);
    }
    QJsonDocument doc;
    doc.setObject(object);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
  }
  else
    return false;
}

bool RoomPropertiesUtility::loadRoomProperties(RoomVariableContainerCollection *variableCollection, ResourceContainerCollection *resourceCollection, const QString &filepath)
{
  QFile file(filepath);
  if(file.open(QIODevice::ReadOnly))
  {
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(),&error);
    file.close();
    if(error.error!=QJsonParseError::NoError)
    {
      return false;
    }
    QJsonObject object = doc.object();
    foreach(QJsonValue value, object[variableString].toArray())
    {
      QJsonObject variableObject = value.toObject();
      variableCollection->handleMessage(variableObject);
    }
    foreach(QJsonValue value, object[resourceString].toArray())
    {
      QJsonObject resourceObject = value.toObject();
      resourceObject.remove("resID");
      resourceCollection->handleMessage(resourceObject);
    }
    return true;
  }
  else
    return false;
}

QList<orpg::Message> RoomPropertiesUtility::loadRoomProperties(const QString &filepath)
{
  QList<orpg::Message> result;
  QFile file(filepath);
  if(file.open(QIODevice::ReadOnly))
  {
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(),&error);
    file.close();
    if(error.error!=QJsonParseError::NoError)
    {
      return result;
    }
    QList<orpg::Message> result;
    QJsonObject object = doc.object();
    foreach(QJsonValue value, object[variableString].toArray())
    {
      QJsonObject variableObject = value.toObject();
      result.append(variableObject);
    }
    foreach(QJsonValue value, object[resourceString].toArray())
    {
      QJsonObject resourceObject = value.toObject();
      resourceObject.remove("resID");
      result.append(resourceObject);
    }
    return result;
  }
  else
    return result;
}
