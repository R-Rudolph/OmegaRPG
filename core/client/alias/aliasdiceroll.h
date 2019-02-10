#ifndef DICEROLL_H
#define DICEROLL_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QPair>

#include <QRegularExpression>

class AliasDiceRoll
{
  const static QString matchesExpressionStrings;
  const static QString parseExpressionStart;
  const static QString parseExpressionContinue;
  bool expanded;
  QString name;
  QString bonusString;
  QString notes;
  bool rollParent;
  QList<AliasDiceRoll> subrolls;

  QPair<int,int> rollFromInfo(const QString& sign,const QString& d,const QString& numDice,const QString& diceValue);
  QList<QPair<int,int>> parsedRolls;
  void parseRollString();
  int parseRollSubString(const QString& string);
public:
  AliasDiceRoll();
  AliasDiceRoll(const QString& name, const QString& bonus, const QList<AliasDiceRoll>& subrolls=QList<AliasDiceRoll>());
  AliasDiceRoll(const QJsonObject& json);
  QJsonObject toJson() const;
  static bool matchesExpression(const QString& string);
  QString getBonusString() const;
  void setBonusString(const QString &value);
  QList<QPair<int, int> > getRolls() const;
  QList<AliasDiceRoll>& getSubrolls();
  void addSubroll(const AliasDiceRoll& roll);
  void clearSubrolls();
  QString getName() const;
  void setName(const QString &value);
  bool getExpanded() const;
  void setExpanded(bool value);
  bool isEmpty();
  QString getNotes() const;
  void setNotes(const QString &value);
  bool getRollParent() const;
  void setRollParent(bool value);
};

#endif // DICEROLL_H
