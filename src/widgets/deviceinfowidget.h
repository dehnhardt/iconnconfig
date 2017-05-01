#ifndef DEVICEINFOWIDGET_H
#define DEVICEINFOWIDGET_H

#include "../device.h"
#include "multiinfowidget.h"

class DeviceInfoWidget : public MultiInfoWidget {

public:
	explicit DeviceInfoWidget(MioMain *parent = 0, Device *device = 0,
														QString windowTitle = tr("Device Information"));
  ~DeviceInfoWidget();

protected:
  void setInfoSections();
};

#endif // DEVICEINFOWIDGET_H
