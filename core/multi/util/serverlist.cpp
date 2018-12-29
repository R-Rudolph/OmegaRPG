#include "serverlist.h"

ServerList::ServerList()
{

}

ServerList::ServerList(QJsonArray serverArray)
{
  for(int i=0;i<serverArray.size();i++)
  {
    entries.push_back(ServerEntry(serverArray[i].toObject()));
  }
}

void ServerList::addEntry(const ServerEntry &entry)
{
  entries.push_back(entry);
}

void ServerList::clear()
{
  entries.clear();
}

int ServerList::size() const
{
  return entries.size();
}

const ServerEntry &ServerList::at(int index) const
{
  return entries.at(index);
}

const ServerEntry &ServerList::operator[](int index) const
{
  return entries[index];
}

QJsonArray ServerList::toJson() const
{
  QJsonArray arr;
  for(int i=0;i<entries.size();i++)
    arr.push_back(entries[i].toJson());
  return arr;
}

bool ServerList::parseXml(const QByteArray &xml)
{
  QDomDocument doc;
  if(doc.setContent(xml))
  {
    QDomElement serversElement = doc.documentElement();
    if(serversElement.tagName()!="servers")
      return false;
    QDomNodeList list = serversElement.elementsByTagName("server");
    for(int i=0;i<list.size();i++)
    {
      ServerEntry server;
      QDomElement elem = list.item(i).toElement();
      if(elem.elementsByTagName("name").count()==0 || elem.elementsByTagName("url").count()==0 || elem.elementsByTagName("players").count()==0)
        continue;
      server.name = elem.elementsByTagName("name").at(0).toElement().text();
      if(server.name.isEmpty())
        server.name = "Unnamed Server";
      server.address = elem.elementsByTagName("url").at(0).toElement().text();
      server.players = elem.elementsByTagName("players").at(0).toElement().text().toInt();
      addEntry(server);
    }
    return true;
  }
  return false;
}

void ServerList::insert(const ServerList &list)
{
  for(int i=list.size()-1;i>=0;i--)
  {
    entries.push_front(list.at(i));
  }
  for(int i=list.size();i<size();i++)
  {
    if(list.contains(entries[i]))
    {
      entries.remove(i);
      i--;
    }
  }
}

bool ServerList::contains(const ServerEntry &entry) const
{
  for(int i=0;i<entries.size();i++)
  {
    if(entry.address == entries[i].address)
      return true;
  }
  return false;
}
