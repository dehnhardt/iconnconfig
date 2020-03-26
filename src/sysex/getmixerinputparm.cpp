#include "getmixerinputparm.h"
#include "retsetmixerinputparm.h"

GetMixerInputParm::GetMixerInputParm(Device *device)
	: PortSysExMessage(GET_MIXER_INPUT_PARM, SysExMessage::QUERY, device) {}

std::vector<unsigned char> *GetMixerInputParm::getMessageData() {
	BYTE_VECTOR *data = new BYTE_VECTOR();
	BYTE_VECTOR *portId = getPortIdBytes();
	data->insert(data->end(), portId->begin(), portId->end());
	data->push_back(static_cast<unsigned char>(m_iMixerInputNumber));
	delete portId;
	return data;
}

void GetMixerInputParm::createAnswer(Command m_Command,
									 std::vector<unsigned char> *message,
									 Device *m_pDevice) {
	m_pAnswer =
		std::make_shared<RetSetMixerInputParm>(m_Command, message, m_pDevice);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}

void GetMixerInputParm::setMixerInputNumber(unsigned int iMixerInputNumber) {
	m_iMixerInputNumber = iMixerInputNumber;
}
