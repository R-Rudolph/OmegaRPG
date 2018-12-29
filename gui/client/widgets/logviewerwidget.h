#ifndef LOGVIEWERWIDGET_H
#define LOGVIEWERWIDGET_H

#include <QWidget>

class QVBoxLayout;
class QTabWidget;
class SearchBar;
class QTextBrowser;

class LogViewerWidget : public QWidget
{
  Q_OBJECT
  QVBoxLayout* layout;
  QTabWidget* tabs;
  SearchBar* searchBar;
public:
  explicit LogViewerWidget(QWidget *parent = nullptr);

signals:

public slots:
  void loadFile(const QString& filepath);
  void showSearchBar();
private slots:
  void search(const QString &text, QRegExp regex, bool backwards, bool caseSensitiveMode, bool regularExpressionMode, bool wholeWordsMode);
};

#endif // LOGVIEWERWIDGET_H
