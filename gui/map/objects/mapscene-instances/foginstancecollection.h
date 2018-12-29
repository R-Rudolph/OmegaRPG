#ifndef FOGINSTANCECOLLECTION_H
#define FOGINSTANCECOLLECTION_H

#include <QMap>
#include <QObject>
#include <QPointF>
#include <QtMath>
#include "foginstance.h"
#include "core/multi/util/player.h"

class QGraphicsItemGroup;

class FogInstanceCollection : public QObject
{
  Q_OBJECT
  qreal gridSize;
  QColor fogColor;
  QString tooltip;
  QGraphicsItemGroup* groupFog;
  QMap<int,FogInstance> instanceMap;
  QMap<PointWrapper,int> positionMap;
  PlayerRole role;
  //utility
  QList<QPointF> getPositions(QPointF position, qreal radius);
  //editing fields
  QMap<PointWrapper,FogInstance> addedFog;
  QMap<int,int> removedIndices;
public:
  explicit FogInstanceCollection(qreal gridSize, QObject *parent = nullptr);
  void addFog(const MapFogResource& resource);
  void removeFog(int id);
  QColor getFogColor() const;
  void setFogColor(const QColor &value);
  void setTooltip(const QString &value);

  void makeFog(QPointF position, qreal radius);
  QGraphicsItemGroup *getItemGroup() const;

  void addFogStart(const QPointF& position, qreal radius);
  void addFogDraw(const QPointF& position, qreal radius);
  void addFogCommit();
  void addFogCancel();

  void removeFogStart(const QPointF& position, qreal radius);
  void removeFogDraw(const QPointF& position, qreal radius);
  void removeFogCommit();
  void removeFogCancel();

  void setPlayerRole(PlayerRole value);
  bool hides(QPointF position);

signals:
  void fogGenerated(QList<MapFogResource> fog);
  void fogRemoved(QList<int> indices);
public slots:
};

#endif // FOGINSTANCECOLLECTION_H
