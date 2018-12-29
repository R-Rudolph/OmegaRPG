#include "distancemeasure.h"
#include <QtMath>

const DistanceMeasure DistanceMeasure::Euclidean = DistanceMeasure(DistanceMeasure::MeasureType::Euclidean);
const DistanceMeasure DistanceMeasure::Manhattan = DistanceMeasure(DistanceMeasure::MeasureType::Manhattan);
const DistanceMeasure DistanceMeasure::DnD = DistanceMeasure(DistanceMeasure::MeasureType::DnD);
bool DistanceMeasure::initializedList = false;
QList<DistanceMeasure> DistanceMeasure::allMeasuresList = QList<DistanceMeasure>();

DistanceMeasure::DistanceMeasure(DistanceMeasure::MeasureType mtype)
{
  this->mtype = mtype;
}

DistanceMeasure::DistanceMeasure() : DistanceMeasure(DistanceMeasure::MeasureType::Euclidean)
{

}

const QList<DistanceMeasure> &DistanceMeasure::getAll()
{
  if(initializedList == false)
  {
    allMeasuresList.append(Euclidean);
    allMeasuresList.append(Manhattan);
    allMeasuresList.append(DnD);
    initializedList = true;
  }
  return allMeasuresList;
}

DistanceMeasure DistanceMeasure::fromInt(int value)
{
  if(value>=0 && value<getAll().size())
    return getAll()[value];
  else
    return Euclidean;
}

int DistanceMeasure::toInt() const
{
  return getAll().indexOf(*this);
}

QString DistanceMeasure::name() const
{
  switch(mtype)
  {
    case MeasureType::Euclidean:
      return "Euclidean Distance";
    case MeasureType::Manhattan:
      return "Manhattan Distance";
    case MeasureType::DnD:
      return "D&D Distance";
    default:
      return "Invalid";
  }
}

QString DistanceMeasure::description() const
{
  switch(mtype)
  {
    case MeasureType::Euclidean:
      return "The shortest distance between two points";
    case MeasureType::Manhattan:
      return "The sum of the X- and Y-Difference between two points";
    case MeasureType::DnD:
      return "The Grid-Distance according to D&D 3.5";
    default:
      return "Invalid";
  }
}

bool DistanceMeasure::operator ==(DistanceMeasure &other) const
{
  return this->mtype == other.mtype;
}

bool DistanceMeasure::operator ==(const DistanceMeasure &other) const
{
  return this->mtype == other.mtype;
}

qreal DistanceMeasure::distanceValue(const QPointF &p1, const QPointF &p2) const
{
  QPointF diff = p1-p2;
  diff.rx() = qAbs(diff.rx());
  diff.ry() = qAbs(diff.ry());
  switch(mtype)
  {
    case MeasureType::Euclidean:
    {
      return qSqrt(QPointF::dotProduct(diff,diff));
    }
    case MeasureType::Manhattan:
    {
      return diff.x() + diff.y();
    }
    case MeasureType::DnD:
    {
      qreal min = qMin(diff.x(),diff.y());
      qreal max= qMax(diff.x(),diff.y());
      int diag = qCeil(min);
      int other = qCeil(max-min);
      return (other + (3*diag)/2 + diag%1)*5;
    }
    default:
      return 0.0;
  }
}

QString DistanceMeasure::distanceString(const QPointF &p1, const QPointF &p2) const
{
  qreal value = ((int)(10*distanceValue(p1,p2)))/10.0;
  switch(mtype)
  {
    case MeasureType::Euclidean:
      return QString::number(value) + " cells";
    case MeasureType::Manhattan:
      return QString::number(value) + " cells";
    case MeasureType::DnD:
      return QString::number(value) + " ft";
    default:
      return "Invalid";
  }
}
