#ifndef IMPLEMENTEDINFOS_H
#define IMPLEMENTEDINFOS_H

#include "sysexmessage.h"

class ImplementedInfos : public SysExMessage {
public:
	ImplementedInfos(Device *device);
	ImplementedInfos(SysExMessage::Command cmd, BYTE_VECTOR *message,
									 Device *device)
			: SysExMessage(cmd, message, device) {}

	std::vector<DeviceInfoItem> *getImplementedInfos() {
		return implementedInfos;
	}
	bool isInfoImplemented(DeviceInfoItem info);
	void parseAnswerData();

private:
	std::vector<DeviceInfoItem> *implementedInfos = 0;
};

#endif // IMPLEMENTEDINFOS_H
