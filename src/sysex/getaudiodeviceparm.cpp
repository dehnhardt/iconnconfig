#include "getaudiodeviceparm.h"
#include "retsetaudiodeviceparm.h"

GetAudioDeviceParm::GetAudioDeviceParm(Device *device)
	: PortSysExMessage(GET_AUDIO_DEVICE_PARM, SysExMessage::QUERY, device) {}

GetAudioDeviceParm::~GetAudioDeviceParm() {}

void GetAudioDeviceParm::createAnswer(Command m_Command,
									  std::vector<unsigned char> *message,
									  Device *m_pDevice) {
	m_pAnswer =
		std::make_shared<RetSetAudioDeviceParm>(m_Command, message, m_pDevice);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}

std::vector<unsigned char> *GetAudioDeviceParm::m_pGetMessageData() {
	BYTE_VECTOR *data = new BYTE_VECTOR();
	BYTE_VECTOR *portId = getPortIdBytes();
	data->insert(data->begin(), portId->begin(), portId->end());
	delete portId;
	return data;
}
