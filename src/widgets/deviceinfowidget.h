#ifndef DEVICEINFOWIDGET_H
#define DEVICEINFOWIDGET_H

#include "../device.h"
#include "../sysex/getinfo.h"
#include "multiinfowidget.h"
#include <QObject>

class DeviceInfoWidget : public MultiInfoWidget {
	Q_OBJECT

public:
  explicit DeviceInfoWidget(MioMain *parent = 0, Device *device = 0,
                            GetInfo *deviceInfo = 0,
                            QString windowTitle = tr("Device Information"));
  ~DeviceInfoWidget();

public slots:
  void deviceInfoChanged(SysExMessage::DeviceInfoItem, std::string);

protected:
  void setInfoSections();
  QWidget *createWidget(MultiInfoListEntry *entry);

private:
  GetInfo *deviceInfo = 0;
};

#endif // DEVICEINFOWIDGET_H
