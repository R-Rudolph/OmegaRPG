#ifndef SEARCHABLETEXTBROWSER_H
#define SEARCHABLETEXTBROWSER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QRegularExpression>
#include "searchbar.h"

class SearchableTextBrowser : public QWidget
{
  Q_OBJECT
  QVBoxLayout* layout_;
  QTextBrowser* browser_;
  SearchBar* searchBar_;
public:
  explicit SearchableTextBrowser(QWidget *parent = 0);
  QTextBrowser* browser();
  SearchBar* searchBar();
signals:

public slots:
  void showSearchBar();
private slots:
  void search(const QString& text, QRegExp regex, bool backwards, bool caseSensitiveMode, bool regularExpressionMode, bool wholeWordsMode);
};

#endif // SEARCHABLETEXTBROWSER_H
