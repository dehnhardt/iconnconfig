#include "getaudiocontroldetailvalue.h"
#include "retsetaudiocontroldetailvalue.h"

GetAudioControlDetailValue::GetAudioControlDetailValue(Device *device)
	: PortSysExMessage(GET_AUDIO_CONTROL_DETAIL_VALUE, SysExMessage::QUERY,
					   device) {}

GetAudioControlDetailValue::~GetAudioControlDetailValue() {}

void GetAudioControlDetailValue::createAnswer(
	Command m_Command, std::vector<unsigned char> *message, Device *m_pDevice) {
	m_pAnswer =
		new RetSetAudioControlDetailValue(m_Command, message, m_pDevice);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}

std::vector<unsigned char> *GetAudioControlDetailValue::m_pGetMessageData() {
	BYTE_VECTOR *data = new BYTE_VECTOR();
	return data;
}
