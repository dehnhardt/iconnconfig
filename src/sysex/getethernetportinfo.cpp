#include "getethernetportinfo.h"
#include "retsetethernetportinfo.h"

GetEthernetPortInfo::GetEthernetPortInfo(Device *device)
	: SysExMessage(Command::GET_ETHERNET_PORT_INFO, SysExMessage::QUERY,
				   device) {}

BYTE_VECTOR *GetEthernetPortInfo::getMessageData() {
	BYTE_VECTOR *messageData = new BYTE_VECTOR();
	messageData->push_back(0x00);
	messageData->push_back(0x01);
	return messageData;
}

void GetEthernetPortInfo::createAnswer(Command cmd, BYTE_VECTOR *message,
									   Device *device) {
	m_pAnswer = std::make_shared<RetSetEthernetPortInfo>(cmd, message, device);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}
