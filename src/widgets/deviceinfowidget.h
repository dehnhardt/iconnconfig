#ifndef DEVICEINFOWIDGET_H
#define DEVICEINFOWIDGET_H

#include "../device.h"
#include "../sysex/deviceinfo.h"
#include "multiinfowidget.h"

class DeviceInfoWidget : public MultiInfoWidget {

public:
	explicit DeviceInfoWidget(MioMain *parent = 0, Device *device = 0,
														DeviceInfo *deviceInfo = 0,
														QString windowTitle = tr("Device Information"));
  ~DeviceInfoWidget();

protected:
	void setInfoSections();
	QWidget *createWidget(std::string infoName);

private:
	DeviceInfo *deviceInfo = 0;
};

#endif // DEVICEINFOWIDGET_H
