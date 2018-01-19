#ifndef DEVICEDETECTIONQUERY_H
#define DEVICEDETECTIONQUERY_H

#include "midi.h"
#include "sysexmessage.h"

class GetDevice : public SysExMessage {
public:
	GetDevice();
	GetDevice(Device *m_pDevice);
	~GetDevice();
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }
};

#endif// DEVICEDETECTIONQUERY_H
