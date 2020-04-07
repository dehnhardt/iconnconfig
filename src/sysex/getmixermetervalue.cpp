#include "getmixermetervalue.h"
#include "retmixermetervalue.h"

GetMixerMeterValue::GetMixerMeterValue(Device *device)
	: PortSysExMessage(GET_MIXER_METER_VALUE, SysExMessage::QUERY, device) {}

GetMixerMeterValue::~GetMixerMeterValue() {}

void GetMixerMeterValue::createAnswer(Command m_Command,
									  std::vector<unsigned char> *message,
									  Device *m_pDevice) {
	m_pAnswer.reset();
	m_pAnswer =
		std::make_shared<RetMixerMeterValue>(m_Command, message, m_pDevice);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}

std::vector<unsigned char> *GetMixerMeterValue::getMessageData() {
	BYTE_VECTOR *data = new BYTE_VECTOR();
	BYTE_VECTOR *portId = getPortIdBytes();
	data->insert(data->begin(), portId->begin(), portId->end());
	data->push_back(static_cast<unsigned char>(m_iMixerOutoutNumber));
	data->push_back(static_cast<unsigned char>(m_portDirection));
	delete portId;
	return data;
}
