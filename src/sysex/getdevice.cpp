#include "getdevice.h"

GetDevice::GetDevice()
	: SysExMessage(Command::GET_DEVICE, SysExMessage::QUERY, 0) {

	m_pTransactionId = new BYTE_VECTOR();
	m_pTransactionId->push_back(0x00);
	m_pTransactionId->push_back(0x01);

	m_pDeviceHeader->push_back(0x00);
	m_pDeviceHeader->push_back(0x00);
	m_pDeviceHeader->push_back(0x00);
	m_pDeviceHeader->push_back(0x00);
	m_pDeviceHeader->push_back(0x00);
	m_pDeviceHeader->push_back(0x00);
	m_pDeviceHeader->push_back(0x00);
}

GetDevice::GetDevice(Device *device)
	: SysExMessage(Command::GET_DEVICE, SysExMessage::QUERY, device) {}

GetDevice::~GetDevice() { delete m_pTransactionId; }
