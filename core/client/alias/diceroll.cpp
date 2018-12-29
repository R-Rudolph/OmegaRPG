#include "diceroll.h"

const QString DiceRoll::matchesExpressionStrings = "^"
                                                   "(\\s*(\\+|\\-)?\\s*((\\d*)d)?(\\d+))?"
                                                   "(\\s*(\\+|\\-)\\s*(((\\d*)d)?(\\d+)))*"
                                                   "\\s*"
                                                   "$";
const QString DiceRoll::parseExpressionStart = "^(\\s*(?<sign>\\+|\\-)?\\s*((?<numDice>\\d*)(?<d>d))?(?<diceValue>\\d+))";
const QString DiceRoll::parseExpressionContinue = "^(\\s*(?<sign>\\+|\\-)\\s*((?<numDice>\\d*)(?<d>d))?(?<diceValue>\\d+))";

QString DiceRoll::getBonusString() const
{
  return bonusString;
}

void DiceRoll::setBonusString(const QString &value)
{
  bonusString = value;
  parseRollString();
}

QString DiceRoll::getName() const
{
  return name;
}

void DiceRoll::setName(const QString &value)
{
  name = value;
}

bool DiceRoll::getExpanded() const
{
  return expanded;
}

void DiceRoll::setExpanded(bool value)
{
  expanded = value;
}

bool DiceRoll::isEmpty()
{
  return name.isEmpty() && bonusString.isEmpty() && subrolls.isEmpty();
}

QString DiceRoll::getNotes() const
{
  return notes;
}

void DiceRoll::setNotes(const QString &value)
{
  notes = value;
}

bool DiceRoll::getRollParent() const
{
  return rollParent;
}

void DiceRoll::setRollParent(bool value)
{
  rollParent = value;
}

QPair<int, int> DiceRoll::rollFromInfo(const QString &sign, const QString &d, const QString &numDice, const QString &diceValue)
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

QList<QPair<int, int> > DiceRoll::getRolls() const
{
  return parsedRolls;
}

QList<DiceRoll>& DiceRoll::getSubrolls()
{
  return subrolls;
}

void DiceRoll::addSubroll(const DiceRoll &roll)
{
  subrolls.append(roll);
}

void DiceRoll::clearSubrolls()
{
  subrolls.clear();
}

void DiceRoll::parseRollString()
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

int DiceRoll::parseRollSubString(const QString &string)
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

DiceRoll::DiceRoll()
{
  rollParent = true;
  expanded = false;
}

DiceRoll::DiceRoll(const QString &name, const QString &roll, const QList<DiceRoll> &subrolls) :DiceRoll()
{
  this->name = name;
  this->bonusString = roll;
  parseRollString();
  this->subrolls = subrolls;
}

DiceRoll::DiceRoll(const QJsonObject &json) :DiceRoll()
{
  rollParent = json["dontRollParent"].toBool(true);
  name = json["name"].toString();
  bonusString = json["bonus"].toString();
  parseRollString();
  QJsonArray subrollArray = json["subrolls"].toArray();
  for(int i=0;i<subrollArray.size();i++)
  {
    subrolls.append(DiceRoll(subrollArray[i].toObject()));
  }
  notes = json["notes"].toString();
}

QJsonObject DiceRoll::toJson() const
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

bool DiceRoll::matchesExpression(const QString &string)
{
  QRegularExpression expr(matchesExpressionStrings);
  return expr.match(string).hasMatch();
}
