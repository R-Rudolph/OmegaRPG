#include "searchabletextbrowser.h"

SearchableTextBrowser::SearchableTextBrowser(QWidget *parent) : QWidget(parent)
{
  layout_ = new QVBoxLayout(this);
  browser_ = new QTextBrowser(this);
  searchBar_ = new SearchBar(this);
  layout_->addWidget(browser_);
  layout_->addWidget(searchBar_);

  connect(searchBar_,&SearchBar::search,this,&SearchableTextBrowser::search);
  searchBar_->hide();
}

QTextBrowser *SearchableTextBrowser::browser()
{
  return browser_;
}

SearchBar *SearchableTextBrowser::searchBar()
{
  return searchBar_;
}

void SearchableTextBrowser::showSearchBar()
{
  searchBar_->show();
  searchBar_->focus();
}

void SearchableTextBrowser::search(const QString &text, QRegExp regex, bool backwards, bool caseSensitiveMode, bool regularExpressionMode, bool wholeWordsMode)
{
  QTextDocument::FindFlags flags = 0;
  if(backwards)
    flags |= QTextDocument::FindBackward;
  if(caseSensitiveMode)
    flags |= QTextDocument::FindCaseSensitively;
  if(wholeWordsMode)
    flags |= QTextDocument::FindWholeWords;
  if(regularExpressionMode)
  {
    if(!browser_->find(regex,flags))
    {
      QTextCursor cursor = browser_->textCursor();
      browser_->setTextCursor(QTextCursor());
      if(!browser_->find(regex,flags))
        browser_->setTextCursor(cursor);
    }
  }
  else
  {
    if(!browser_->find(text,flags))
    {
      QTextCursor cursor = browser_->textCursor();
      browser_->setTextCursor(QTextCursor());
      if(!browser_->find(text,flags))
        browser_->setTextCursor(cursor);
    }
  }
}
