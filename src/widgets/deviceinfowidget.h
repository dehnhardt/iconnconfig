#ifndef DEVICEINFOWIDGET_H
#define DEVICEINFOWIDGET_H

#include "../device.h"
#include "../sysex/getinfo.h"
#include "multiinfowidget.h"
#include <QObject>

class DeviceInfoWidget : public MultiInfoWidget {
	Q_OBJECT

  public:
	explicit DeviceInfoWidget(MioMain *parent = nullptr,
							  Device *device = nullptr,
							  std::shared_ptr<GetInfo> m_pDeviceInfo = nullptr,
							  QString windowTitle = tr("Device Information"));
	~DeviceInfoWidget();

  public slots:
	void deviceInfoChanged(SysExMessage::DeviceInfoItem, std::string);

  protected:
	void setInfoSections();
	QWidget *createWidget(MultiInfoListEntry *entry);

  private:
	std::shared_ptr<GetInfo> m_pDeviceInfo = nullptr;
};

#endif // DEVICEINFOWIDGET_H
