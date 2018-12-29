#ifndef TEXTEDITHTMLINSERT_H
#define TEXTEDITHTMLINSERT_H

#include <QWidget>
#include <QTextEdit>
#include <QMimeData>

#include <QRegularExpression>

class TextEditHtmlInsert : public QTextEdit
{
  Q_OBJECT
public:
  explicit TextEditHtmlInsert(QWidget *parent = 0);
protected:
  virtual void insertFromMimeData(const QMimeData *source);
  virtual QMimeData* createMimeDataFromSelection() const;
signals:

public slots:
};

#endif // TEXTEDITHTMLINSERT_H
