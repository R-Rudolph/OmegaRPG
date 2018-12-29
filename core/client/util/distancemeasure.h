#ifndef DISTANCEMEASURE_H
#define DISTANCEMEASURE_H

#include<QList>
#include<QString>
#include<QPointF>

class DistanceMeasure
{
  enum class MeasureType
  {
    Euclidean,
    Manhattan,
    DnD
  };
  MeasureType mtype;
  DistanceMeasure(MeasureType mtype);
  static bool initializedList;
  static QList<DistanceMeasure> allMeasuresList;
public:
  DistanceMeasure();
  const static DistanceMeasure Euclidean;
  const static DistanceMeasure Manhattan;
  const static DistanceMeasure DnD;
  const static QList<DistanceMeasure>& getAll();
  static DistanceMeasure fromInt(int value);
  int toInt() const;
  QString name() const;
  QString description() const;
  bool operator ==(DistanceMeasure& other) const;
  bool operator ==(const DistanceMeasure& other) const;
  qreal distanceValue(const QPointF& p1, const QPointF& p2) const;
  QString distanceString(const QPointF& p1, const QPointF& p2) const;
};

#endif // DISTANCEMEASURE_H
