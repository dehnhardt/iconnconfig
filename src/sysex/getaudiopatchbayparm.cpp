#include "getaudiopatchbayparm.h"
#include "retsetaudiopatchbayparm.h"

GetAudioPatchbayParm::GetAudioPatchbayParm(Device *device)
	: PortSysExMessage(GET_AUDIO_PATCHBAY_PARM, SysExMessage::QUERY, device) {}

GetAudioPatchbayParm::~GetAudioPatchbayParm() {}

void GetAudioPatchbayParm::createAnswer(Command m_Command,
										std::vector<unsigned char> *message,
										Device *m_pDevice) {
	m_pAnswer = std::make_shared<RetSetAudioPatchbayParm>(m_Command, message,
														  m_pDevice);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}

std::vector<unsigned char> *GetAudioPatchbayParm::getMessageData() {
	BYTE_VECTOR *data = new BYTE_VECTOR();
	BYTE_VECTOR *portId = getPortIdBytes();
	data->insert(data->begin(), portId->begin(), portId->end());
	delete portId;
	return data;
}
