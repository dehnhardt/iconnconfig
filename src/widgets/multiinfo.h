#ifndef MULTIINFO_H
#define MULTIINFO_H

#include <QWidget>

class MultiInfo : public QObject {
  Q_OBJECT
public:
  explicit MultiInfo(QWidget *parent = 0);

protected:
  std::vector<std::string> infoSections;
  std::map<std::string, QWidget *> mainWidgets;
  QWidget *createWidget(std::string widgetName);

signals:

public slots:
};

#endif // MULTIINFO_H
