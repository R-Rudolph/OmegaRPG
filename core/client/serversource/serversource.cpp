#include "serversource.h"

ServerSource::ServerSource(QObject *parent) : QObject(parent)
{

}

void ServerSource::execContextMenu(QTreeWidgetItem*,const QPoint&)
{

}

QTreeWidgetItem *ServerSource::getTreeWidgetItem()
{
  return nullptr;
}
