#ifndef DEVICEINFOWIDGET_H
#define DEVICEINFOWIDGET_H

#include "../device.h"
#include "multiinfowidget.h"

#include <QDockWidget>

namespace Ui {
class DeviceInfoWidget;
}

class DeviceInfoWidget : public MultiInfoWidget {

public:
  explicit DeviceInfoWidget(QWidget *parent = 0, Device *device = 0);
  ~DeviceInfoWidget();
};

#endif // DEVICEINFOWIDGET_H
