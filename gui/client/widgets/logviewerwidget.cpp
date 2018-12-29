#include "logviewerwidget.h"

#include<QVBoxLayout>
#include<QTabWidget>
#include<QTextBrowser>
#include<QLabel>
#include "widgets/searchbar.h"
#include "core/client/log/sessionlogparser.h"
#include "core/client/log/jsonlogparser.h"

LogViewerWidget::LogViewerWidget(QWidget *parent) : QWidget(parent)
{
  layout = new QVBoxLayout(this);
  tabs = new QTabWidget();
  searchBar = new SearchBar();

  tabs->setTabShape(QTabWidget::Rounded);
  tabs->tabBar()->setStyleSheet("QTabBar"
                                "{"
                                "font-weight: bold"
                                "}");

  layout->addWidget(tabs,1);
  layout->addWidget(searchBar,0);
  searchBar->setHidden(true);
  connect(searchBar,&SearchBar::search,this,&LogViewerWidget::search);
}

void LogViewerWidget::loadFile(const QString& filepath)
{
  //remove previous channel tabs
  while(tabs->count()>0)
  {
    delete tabs->widget(0);
  }
  //add tabs from json parser
  {
    orpg::JsonLogParser parser(filepath);
    QList< QPair<QString,QString> > channels = parser.getHtmlLogByChannels();
    for(int i=0;i<channels.size();i++)
    {
      QTextBrowser* browser = new QTextBrowser(this);
      browser->setHtml(channels[i].second);
      tabs->addTab(browser,channels[i].first);
    }
    return;
  }
  //load file into file parser, extract channels
  {
    QMap<QString,QTextBrowser*> map;
    orpg::SessionLogParser parser(filepath);
    if(parser.isValid())
    {
      QList<orpg::ChatMessageForLog> messages = parser.getMessages();
      foreach(const orpg::ChatMessageForLog& message, messages)
      {
        QString channel = parser.getChannelName(message.getChannel());
        if(!map.contains(channel))
        {
          QTextBrowser* browser = new QTextBrowser(this);
          map.insert(channel,browser);
          if(channel!=orpg::SessionLogParser::mainChannelString)
            tabs->addTab(browser,channel);
        }
        {
          QTextBrowser* browser = map[channel];
          browser->append(message.toHtml());
        }
      }
      if(map.contains(orpg::SessionLogParser::mainChannelString))
        tabs->insertTab(0,map[orpg::SessionLogParser::mainChannelString],orpg::SessionLogParser::mainChannelString);
    }
    else
    {
      tabs->addTab(new QLabel("Error: Could not parse log file."),"Error");
    }
  }
  //add mixed channels tab (file view)
  {
    QTextBrowser* browser = new QTextBrowser();
    browser->setSource(QUrl::fromLocalFile(filepath));
    tabs->addTab(browser,"File");
  }
}

void LogViewerWidget::showSearchBar()
{
  searchBar->show();
  searchBar->focus();
}

void LogViewerWidget::search(const QString &text, QRegExp regex, bool backwards, bool caseSensitiveMode, bool regularExpressionMode, bool wholeWordsMode)
{
  QTextBrowser* browser = dynamic_cast<QTextBrowser*>(tabs->currentWidget());
  if(browser==nullptr)
    return;
  QTextDocument::FindFlags flags = 0;
  if(backwards)
    flags |= QTextDocument::FindBackward;
  if(caseSensitiveMode)
    flags |= QTextDocument::FindCaseSensitively;
  if(wholeWordsMode)
    flags |= QTextDocument::FindWholeWords;
  if(regularExpressionMode)
  {
    if(!browser->find(regex,flags))
    {
      QTextCursor cursor = browser->textCursor();
      browser->setTextCursor(QTextCursor());
      if(!browser->find(regex,flags))
        browser->setTextCursor(cursor);
    }
  }
  else
  {
    if(!browser->find(text,flags))
    {
      QTextCursor cursor = browser->textCursor();
      browser->setTextCursor(QTextCursor());
      if(!browser->find(text,flags))
        browser->setTextCursor(cursor);
    }
  }
}
