#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QRegularExpression>

class SearchBar : public QWidget
{
  Q_OBJECT
  static const QString prevNextStyleSheet;
  QHBoxLayout* layout;
  QLineEdit* stringEdit;
  QPushButton* nextButton;
  QPushButton* prevButton;
  QPushButton* caseSensitiveSwitch;
  QPushButton* wholeWordsSwitch;
  QPushButton* regularExpressionSwitch;
  QPushButton* hideButton;

  QRegExp regEx;
  void sendSearch(bool backwards);
public:
  explicit SearchBar(QWidget *parent = 0);
  void focus();
signals:
  void search(const QString& text, QRegExp regex, bool backwards, bool caseSensitiveMode, bool regularExpressionMode, bool wholeWordsMode);
private slots:
  void updateInfo();
  void searchNext();
  void searchPrev();
};

#endif // SEARCHBAR_H
