#include "retsetinfo.h"

RetSetInfo::RetSetInfo(Device *device)
		: RetSetInfo(SysExMessage::QUERY, device) {}

RetSetInfo::RetSetInfo(CommandFlags flags, Device *device)
		: SysExMessage(Command::RET_SET_INFO, flags, device) {}

SysExMessage::DeviceInfoItem RetSetInfo::getInfoItem() { return m_InfoItem; }

std::string RetSetInfo::getValue() { return m_sValue; }

void RetSetInfo::parseAnswerData() {
	this->m_InfoItem = (SysExMessage::DeviceInfoItem)m_pData->at(0);
	this->m_sValue = std::string(m_pData->begin() + 1, m_pData->end());
}

std::vector<unsigned char> *RetSetInfo::m_pGetMessageData() {
	BYTE_VECTOR *messageData = new BYTE_VECTOR();
	messageData->push_back(this->m_InfoItem);
	messageData->insert(messageData->end(), m_sValue.begin(), m_sValue.end());
	return messageData;
}

bool RetSetInfo::isItemEditable() {
	switch (m_InfoItem) {
	case DEVICE_NAME:
		return true;
	default:
		return false;
	}
}

std::string RetSetInfo::getItemName() {
	switch (m_InfoItem) {
	case ACCESSORY_NAME:
		return tr("Accessory Name").toStdString();
	case MANUFACTURER_NAME:
		return tr("Manufacturer Name").toStdString();
	case MODEL_NUMBER:
		return tr("Model Number").toStdString();
	case SERIAL_NUMBER:
		return tr("Serial Number").toStdString();
	case FIRMWARE_VERSION:
		return tr("Firmware Version").toStdString();
	case HARDWARE_VERSION:
		return tr("Hardware Version").toStdString();
	case DEVICE_NAME:
		return tr("Name").toStdString();
	default:
		return tr("unknown").toStdString();
	}
}

bool RetSetInfo::setValue(std::string value) {
	this->m_sValue = value;
	this->m_pCommandData->at(0) = 0x40;
	execute();
	return true;
}
