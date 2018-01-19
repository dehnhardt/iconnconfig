#ifndef IMPLEMENTEDINFOS_H
#define IMPLEMENTEDINFOS_H

#include "sysexmessage.h"

class RetInfoList : public SysExMessage {
public:
	RetInfoList(Device *m_pDevice);
	RetInfoList(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}

	std::vector<DeviceInfoItem> *getImplementedInfos() {
		return m_pImplementedInfos;
	}
	bool isInfoImplemented(DeviceInfoItem info);
	void parseAnswerData();
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

private:
	std::vector<DeviceInfoItem> *m_pImplementedInfos = 0;
};

#endif// IMPLEMENTEDINFOS_H
