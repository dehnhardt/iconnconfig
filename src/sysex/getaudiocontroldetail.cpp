#include "getaudiocontroldetail.h"
#include "retsetaudiocontroldetail.h"

GetAudioControlDetail::GetAudioControlDetail(Device *device)
    : PortSysExMessage(GET_AUDIO_CONTROL_DETAIL, SysExMessage::QUERY, device) {}

GetAudioControlDetail::~GetAudioControlDetail() {}

void GetAudioControlDetail::createAnswer(Command m_Command,
                                         std::vector<unsigned char> *message,
                                         Device *m_pDevice) {
	m_pAnswer = std::make_shared<RetSetAudioControlDetail>(m_Command, message,
	                                                       m_pDevice);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}

std::vector<unsigned char> *GetAudioControlDetail::getMessageData() {
	BYTE_VECTOR *data = new BYTE_VECTOR();
	BYTE_VECTOR *portId = getPortIdBytes();
	data->insert(data->begin(), portId->begin(), portId->end());
	data->push_back(static_cast<unsigned char>(m_iControllerNumber));
	data->push_back(static_cast<unsigned char>(m_iDetailNumber));
	delete portId;
	return data;
}

void GetAudioControlDetail::setDetailNumber(unsigned int iDetailNumber) {
	m_iDetailNumber = iDetailNumber;
}

void GetAudioControlDetail::setControllerNumber(
    unsigned int iControllerNumber) {
	m_iControllerNumber = iControllerNumber;
}
