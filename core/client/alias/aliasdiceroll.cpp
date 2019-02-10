#include "aliasdiceroll.h"

const QString AliasDiceRoll::matchesExpressionStrings = "^"
                                                   "(\\s*(\\+|\\-)?\\s*((\\d*)d)?(\\d+))?"
                                                   "(\\s*(\\+|\\-)\\s*(((\\d*)d)?(\\d+)))*"
                                                   "\\s*"
                                                   "$";
const QString AliasDiceRoll::parseExpressionStart = "^(\\s*(?<sign>\\+|\\-)?\\s*((?<numDice>\\d*)(?<d>d))?(?<diceValue>\\d+))";
const QString AliasDiceRoll::parseExpressionContinue = "^(\\s*(?<sign>\\+|\\-)\\s*((?<numDice>\\d*)(?<d>d))?(?<diceValue>\\d+))";

QString AliasDiceRoll::getBonusString() const
{
  return bonusString;
}

void AliasDiceRoll::setBonusString(const QString &value)
{
  bonusString = value;
  parseRollString();
}

QString AliasDiceRoll::getName() const
{
  return name;
}

void AliasDiceRoll::setName(const QString &value)
{
  name = value;
}

bool AliasDiceRoll::getExpanded() const
{
  return expanded;
}

void AliasDiceRoll::setExpanded(bool value)
{
  expanded = value;
}

bool AliasDiceRoll::isEmpty()
{
  return name.isEmpty() && bonusString.isEmpty() && subrolls.isEmpty();
}

QString AliasDiceRoll::getNotes() const
{
  return notes;
}

void AliasDiceRoll::setNotes(const QString &value)
{
  notes = value;
}

bool AliasDiceRoll::getRollParent() const
{
  return rollParent;
}

void AliasDiceRoll::setRollParent(bool value)
{
  rollParent = value;
}

QPair<int, int> AliasDiceRoll::rollFromInfo(const QString &sign, const QString &d, const QString &numDice, const QString &diceValue)
{
  QPair<int,int> result;
  bool ok;
  result.first = numDice.toInt(&ok);
  if(!ok)
  {
    if(d.isEmpty())
      result.first = 0;
    else
      result.first = 1;
  }
  result.second = diceValue.toInt();
  if(sign == "-")
    result.second = -result.second;
  return result;
}

QList<QPair<int, int> > AliasDiceRoll::getRolls() const
{
  return parsedRolls;
}

QList<AliasDiceRoll>& AliasDiceRoll::getSubrolls()
{
  return subrolls;
}

void AliasDiceRoll::addSubroll(const AliasDiceRoll &roll)
{
  subrolls.append(roll);
}

void AliasDiceRoll::clearSubrolls()
{
  subrolls.clear();
}

void AliasDiceRoll::parseRollString()
{
  QString string = bonusString;
  parsedRolls.clear();
  QRegularExpression regex(parseExpressionStart);
  QRegularExpressionMatch match = regex.match(string);
  if(match.hasMatch())
  {
    QPair<int,int> newRoll = rollFromInfo(match.captured("sign"),match.captured("d"),match.captured("numDice"),match.captured("diceValue"));
    parsedRolls.append(newRoll);
    string.remove(0,match.capturedEnd(0));
    int captureEnd = 1;
    while(captureEnd>0)
    {
      captureEnd = parseRollSubString(string);
      string.remove(0,captureEnd);
    }
  }
}

int AliasDiceRoll::parseRollSubString(const QString &string)
{
  QRegularExpression regex(parseExpressionContinue);
  QRegularExpressionMatch match = regex.match(string);
  if(match.hasMatch())
  {
    QPair<int,int> newRoll = rollFromInfo(match.captured("sign"),match.captured("d"),match.captured("numDice"),match.captured("diceValue"));
    parsedRolls.append(newRoll);
    return match.capturedEnd(0);
  }
  else
    return 0;
}

AliasDiceRoll::AliasDiceRoll()
{
  rollParent = true;
  expanded = false;
}

AliasDiceRoll::AliasDiceRoll(const QString &name, const QString &roll, const QList<AliasDiceRoll> &subrolls) :AliasDiceRoll()
{
  this->name = name;
  this->bonusString = roll;
  parseRollString();
  this->subrolls = subrolls;
}

AliasDiceRoll::AliasDiceRoll(const QJsonObject &json) :AliasDiceRoll()
{
  rollParent = json["dontRollParent"].toBool(true);
  name = json["name"].toString();
  bonusString = json["bonus"].toString();
  parseRollString();
  QJsonArray subrollArray = json["subrolls"].toArray();
  for(int i=0;i<subrollArray.size();i++)
  {
    subrolls.append(AliasDiceRoll(subrollArray[i].toObject()));
  }
  notes = json["notes"].toString();
}

QJsonObject AliasDiceRoll::toJson() const
{
  QJsonObject result;
  result.insert("dontRollParent",rollParent);
  result.insert("name",name);
  result.insert("bonus",bonusString);
  QJsonArray subrollArray;
  for(int i=0;i<subrolls.size();i++)
  {
    subrollArray.append(subrolls[i].toJson());
  }
  result.insert("subrolls",subrollArray);
  result.insert("notes",notes);
  return result;
}

bool AliasDiceRoll::matchesExpression(const QString &string)
{
  QRegularExpression expr(matchesExpressionStrings);
  return expr.match(string).hasMatch();
}
