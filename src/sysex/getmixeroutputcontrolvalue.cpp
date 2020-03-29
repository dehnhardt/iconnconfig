#include "getmixeroutputcontrolvalue.h"
#include "retsetmixeroutputcontrolvalue.h"

GetMixerOutputControlValue::GetMixerOutputControlValue(Device *device)
	: PortSysExMessage(GET_MIXER_OUTPUT_CONTROL_VALUE, SysExMessage::QUERY,
					   device) {}

std::vector<unsigned char> *GetMixerOutputControlValue::getMessageData() {
	BYTE_VECTOR *data = new BYTE_VECTOR();
	BYTE_VECTOR *portId = getPortIdBytes();
	data->insert(data->end(), portId->begin(), portId->end());
	data->push_back(static_cast<unsigned char>(m_iMixerOutputNumber));
	delete portId;
	return data;
}

void GetMixerOutputControlValue::createAnswer(
	Command command, std::vector<unsigned char> *message, Device *device) {
	m_pAnswer = std::make_shared<RetSetMixerOutputControlValue>(
		command, message, device);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}

void GetMixerOutputControlValue::setMixerOutputNumber(
	unsigned int iMixerOutputNumber) {
	m_iMixerOutputNumber = iMixerOutputNumber;
}
