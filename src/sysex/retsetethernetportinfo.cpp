#include "retsetethernetportinfo.h"
#include "sstream"

RetSetEthernetPortInfo::RetSetEthernetPortInfo(Device *device)
	: SysExMessage(RET_SET_ETHERNET_PORT_INFO, QUERY, device) {
	m_pAddresses = new std::map<int, std::string>();
}

void RetSetEthernetPortInfo::parseAnswerData() {
	std::cout << "parseAnswerData" << std::endl;
	m_iCommandVersionNumber = m_pData->at(0);
	m_pPortId = new BYTE_VECTOR(m_pData->begin() + 1, m_pData->begin() + 3);
	m_MethodFlag = static_cast<IPFlags>(m_pData->at(3));

	setAddress(STATIC | ADDRESS, MIDI::decodeIp(m_pData, 4));
	setAddress(STATIC | SUBNET_MASK, MIDI::decodeIp(m_pData, 9));
	setAddress(STATIC | GATEWAY, MIDI::decodeIp(m_pData, 14));
	setAddress(DYNAMIC | ADDRESS, MIDI::decodeIp(m_pData, 19));
	setAddress(DYNAMIC | SUBNET_MASK, MIDI::decodeIp(m_pData, 24));
	setAddress(DYNAMIC | GATEWAY, MIDI::decodeIp(m_pData, 29));
	m_sMacAddress = std::string(m_pData->begin() + 34, m_pData->begin() + 46);
	int bonjourNameLength = m_pData->at(46);
	m_sBonjourName = std::string(m_pData->begin() + 47,
								 m_pData->begin() + 47 + bonjourNameLength);
}

std::string RetSetEthernetPortInfo::getAddress(int flags) {
	return (*m_pAddresses)[flags];
}

void RetSetEthernetPortInfo::setAddress(int flags, std::string address) {
	(*m_pAddresses)[flags] = address;
}

std::string RetSetEthernetPortInfo::getMacAddress() const {
	return m_sMacAddress;
}

std::string RetSetEthernetPortInfo::getBonjourName() const {
	return m_sBonjourName;
}

void RetSetEthernetPortInfo::setBonjourName(const std::string &value) {
	m_sBonjourName = value;
}

RetSetEthernetPortInfo::IPFlags RetSetEthernetPortInfo::getMethod() {
	return m_MethodFlag;
}

void RetSetEthernetPortInfo::setMethod(RetSetEthernetPortInfo::IPFlags method) {
	this->m_MethodFlag = method;
}

std::vector<unsigned char> *RetSetEthernetPortInfo::getMessageData() {
	this->m_pCommandData->at(0) = 0x40;
	BYTE_VECTOR *messageData = new BYTE_VECTOR();
	messageData->push_back(m_iCommandVersionNumber);
	messageData->insert(messageData->end(), m_pPortId->begin(),
						m_pPortId->end());
	messageData->push_back(m_MethodFlag);
	BYTE_VECTOR *ip = MIDI::encodeIpAddress(getAddress(STATIC | ADDRESS));
	messageData->insert(messageData->end(), ip->begin(), ip->end());
	ip = MIDI::encodeIpAddress(getAddress(STATIC | SUBNET_MASK));
	messageData->insert(messageData->end(), ip->begin(), ip->end());
	ip = MIDI::encodeIpAddress(getAddress(STATIC | GATEWAY));
	messageData->insert(messageData->end(), ip->begin(), ip->end());
	ip = MIDI::encodeIpAddress(getAddress(DYNAMIC | ADDRESS));
	messageData->insert(messageData->end(), ip->begin(), ip->end());
	ip = MIDI::encodeIpAddress(getAddress(DYNAMIC | SUBNET_MASK));
	messageData->insert(messageData->end(), ip->begin(), ip->end());
	ip = MIDI::encodeIpAddress(getAddress(DYNAMIC | GATEWAY));
	messageData->insert(messageData->end(), ip->begin(), ip->end());
	messageData->insert(messageData->end(), m_sMacAddress.begin(),
						m_sMacAddress.end());
	messageData->push_back(static_cast<unsigned char>(m_sBonjourName.size()));
	messageData->insert(messageData->end(), m_sBonjourName.begin(),
						m_sBonjourName.end());
	return messageData;
}
