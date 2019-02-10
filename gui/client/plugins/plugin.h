#ifndef PLUGIN_H
#define PLUGIN_H

#include <QObject>
#include <QQuickItem>
#include <QJsonObject>

class Plugin : public QQuickItem
{
  Q_OBJECT
  Q_PROPERTY(QString pluginName READ pluginName WRITE setPluginName)
  Q_PROPERTY(QString description READ description WRITE setDescription)
  Q_PROPERTY(QJsonObject pluginData READ pluginData WRITE setPluginData NOTIFY pluginDataSet)
  Q_PROPERTY(bool editable READ editable WRITE setEditable NOTIFY editableChanged)
public:
  explicit Plugin(QQuickItem *parent = nullptr);
  QString pluginName() const;
  void setPluginName(const QString& pluginName);
  QString description() const;
  void setDescription(const QString& description);
  QJsonObject pluginData() const;
  void setPluginData(const QJsonObject& pluginData);
  bool editable() const;
  void setEditable(bool editable);
  Q_INVOKABLE void roll(const QString& rollString);
signals:
  void editableChanged();
  void pluginDataSet();
  void pluginDataChanged(const QJsonObject& data);
  void chat(const QString& text);
public slots:
private:
  bool editable_;
  QString pluginName_;
  QString description_;
  QJsonObject pluginData_;
};

#endif // PLUGIN_H
