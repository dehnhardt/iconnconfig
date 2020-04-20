#include "retsetaudiodeviceparm.h"

RetSetAudioDeviceParm::RetSetAudioDeviceParm(Device *device)
	: PortSysExMessage(RET_SET_AUDIO_DEVICE_PARM, SysExMessage::QUERY, device) {

}

RetSetAudioDeviceParm::~RetSetAudioDeviceParm() {}

void RetSetAudioDeviceParm::parseAnswerData() {
	m_iCommandVersionNumber = m_pData->at(0);
	m_iPortId = static_cast<unsigned int>(MIDI::byteJoin7bit(m_pData, 1, 2));
	m_AudioPortType = static_cast<pk::AudioPortType>(m_pData->at(3));
	m_iMaxControllers = m_pData->at(4);
	unsigned long offset = 5;
	switch (m_AudioPortType) {
	case pk::AudioPortType::APT_USB_DEVICE:
		m_usbDeviceHost = static_cast<pk::UsbDeviceHost>(m_pData->at(offset));
		offset++;
		m_iHostNameLength = m_pData->at(offset);
		offset++;
		if (m_iHostNameLength > 0)
			m_sHostName =
				std::string(m_pData->begin() + static_cast<long>(offset),
							m_pData->begin() + static_cast<long>(offset) +
								m_iHostNameLength);
		break;
	case pk::AudioPortType::APT_USB_HOST: {
		int flags = m_pData->at(offset);
		m_bHostConnected = flags & 32;
		m_bPortReservedForVendorProduct = flags & 1;
		break;
	}
	default:
		break;
	}
}

unsigned int RetSetAudioDeviceParm::getMaxControllers() const {
	return m_iMaxControllers;
}

bool RetSetAudioDeviceParm::getHostConnected() const {
	return m_bHostConnected;
}

bool RetSetAudioDeviceParm::getPortReservedForVendorProduct() const {
	return m_bPortReservedForVendorProduct;
}

void RetSetAudioDeviceParm::setPortReservedForVendorProduct(
	bool bPortReservedForVendorProduct) {
	m_bPortReservedForVendorProduct = bPortReservedForVendorProduct;
}

std::string RetSetAudioDeviceParm::getHostName() const { return m_sHostName; }

pk::UsbDeviceHost RetSetAudioDeviceParm::getUsbDeviceHost() const {
	return m_usbDeviceHost;
}
