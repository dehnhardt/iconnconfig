#ifndef DEVICEINFOWIDGET_H
#define DEVICEINFOWIDGET_H

#include "../device.h"

#include <QDockWidget>

namespace Ui {
class DeviceInfoWidget;
}

class DeviceInfoWidget : public QDockWidget {
  Q_OBJECT

public:
  explicit DeviceInfoWidget(QWidget *parent = 0, Device *device = 0);
  ~DeviceInfoWidget();

private:
  Ui::DeviceInfoWidget *ui;
  Device *device;
};

#endif // DEVICEINFOWIDGET_H
