#include "getaudiochannelname.h"
#include "retsetaudiochannelname.h"

GetAudioChannelName::GetAudioChannelName(Device *device)
	: PortSysExMessage(GET_AUDIO_CHANNEL_NAME, SysExMessage::QUERY, device) {}

GetAudioChannelName::~GetAudioChannelName() {}

void GetAudioChannelName::createAnswer(Command m_Command,
									   std::vector<unsigned char> *message,
									   Device *m_pDevice) {
	m_pAnswer =
		std::make_shared<RetSetAudioChannelName>(m_Command, message, m_pDevice);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}

void GetAudioChannelName::setChannelNumber(unsigned int iChannelNumber) {
	m_iChannelNumber = iChannelNumber;
}

void GetAudioChannelName::setChannelDirection(
	const ChannelDirection &channelDirection) {
	m_channelDirection = channelDirection;
}

std::vector<unsigned char> *GetAudioChannelName::getMessageData() {
	BYTE_VECTOR *data = new BYTE_VECTOR();
	BYTE_VECTOR *portId = getPortIdBytes();
	data->insert(data->begin(), portId->begin(), portId->end());
	data->push_back(static_cast<unsigned char>(m_iChannelNumber));
	data->push_back(static_cast<unsigned char>(m_channelDirection));
	delete portId;
	return data;
}
