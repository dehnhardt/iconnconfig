#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include "../device.h"

#include <QDockWidget>
#include <QGridLayout>
#include <QObject>
#include <QWidget>

class CentralWidget : public QDockWidget {
  Q_OBJECT

public:
  CentralWidget(QWidget *parent, Device *device);
  void replacePanel(QWidget *w);

private:
  QWidget *currentWidget = 0;
  Device *device;
  QGridLayout *l = 0;
};

#endif // CENTRALWIDGET_H
