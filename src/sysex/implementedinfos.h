#ifndef IMPLEMENTEDINFOS_H
#define IMPLEMENTEDINFOS_H

#include "sysexmessage.h"

class ImplementedInfos : public SysExMessage {
public:
	ImplementedInfos(Device *device);
	ImplementedInfos(SysExMessage::Command cmd, BYTE_VECTOR *message,
									 Device *device)
			: SysExMessage(cmd, message, device) {}
	std::vector<DeviceInfoItem> getSupportedInfos();
	bool isInfoSupported(DeviceInfoItem info);
	void parseAnswerData();

private:
	std::vector<DeviceInfoItem> *supportedInfos = 0;
};

#endif // IMPLEMENTEDINFOS_H
