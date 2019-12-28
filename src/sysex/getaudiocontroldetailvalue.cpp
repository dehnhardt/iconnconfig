#include "getaudiocontroldetailvalue.h"
#include "retsetaudiocontroldetailvalue.h"

GetAudioControlDetailValue::GetAudioControlDetailValue(Device *device)
    : PortSysExMessage(GET_AUDIO_CONTROL_DETAIL_VALUE, SysExMessage::QUERY,
                       device) {}

GetAudioControlDetailValue::~GetAudioControlDetailValue() {}

void GetAudioControlDetailValue::createAnswer(
    Command m_Command, std::vector<unsigned char> *message, Device *m_pDevice) {
	m_pAnswer = std::make_shared<RetSetAudioControlDetailValue>(
	    m_Command, message, m_pDevice);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}

std::vector<unsigned char> *GetAudioControlDetailValue::getMessageData() {
	BYTE_VECTOR *data = new BYTE_VECTOR();
	BYTE_VECTOR *portId = getPortIdBytes();
	data->insert(data->begin(), portId->begin(), portId->end());
	data->push_back(static_cast<unsigned char>(m_iControllerNumber));
	data->push_back(static_cast<unsigned char>(m_iDetailNumber));
	delete portId;
	return data;
}

unsigned char GetAudioControlDetailValue::getDetailNumber() const {
	return m_iDetailNumber;
}

void GetAudioControlDetailValue::setDetailNumber(unsigned char iDetailNumber) {
	m_iDetailNumber = iDetailNumber;
}

unsigned char GetAudioControlDetailValue::getControllerNumber() const {
	return m_iControllerNumber;
}

void GetAudioControlDetailValue::setControllerNumber(
    unsigned char iControllerNumber) {
	m_iControllerNumber = iControllerNumber;
}
