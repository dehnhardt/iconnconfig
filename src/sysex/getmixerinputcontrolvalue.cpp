#include "getmixerinputcontrolvalue.h"
#include "retsetmixerinputcontrolvalue.h"

GetMixerInputControlValue::GetMixerInputControlValue(Device *device)
	: PortSysExMessage(GET_MIXER_INPUT_CONTROL_VALUE, SysExMessage::QUERY,
					   device) {}

std::vector<unsigned char> *GetMixerInputControlValue::getMessageData() {
	BYTE_VECTOR *data = new BYTE_VECTOR();
	BYTE_VECTOR *portId = getPortIdBytes();
	data->insert(data->end(), portId->begin(), portId->end());
	data->push_back(static_cast<unsigned char>(m_iMixerOutputNumber));
	data->push_back(static_cast<unsigned char>(m_iMixerInputNumber));
	delete portId;
	return data;
}

void GetMixerInputControlValue::createAnswer(
	Command m_Command, std::vector<unsigned char> *message, Device *m_pDevice) {
	m_pAnswer = std::make_shared<RetSetMixerInputControlValue>(
		m_Command, message, m_pDevice);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}

void GetMixerInputControlValue::setMixerOutputNumber(
	unsigned int iMixerOutputNumber) {
	m_iMixerOutputNumber = iMixerOutputNumber;
}

void GetMixerInputControlValue::setMixerInputNumber(
	unsigned int iMixerInputNumber) {
	m_iMixerInputNumber = iMixerInputNumber;
}
