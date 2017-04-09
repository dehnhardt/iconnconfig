#ifndef DEVICEDOCKWIDGET_H
#define DEVICEDOCKWIDGET_H

#include "../device.h"

#include <QDockWidget>
#include <QObject>

class DeviceDockWidget : public QDockWidget {
  Q_OBJECT
public:
  explicit DeviceDockWidget(QWidget *parent = 0, Device *device = 0);
  ~DeviceDockWidget();

private:
  Device *device;
};

#endif // DEVICEDOCKWIDGET_H
