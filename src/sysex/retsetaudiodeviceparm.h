#ifndef RETSETAUDIODEVICEPARM_H
#define RETSETAUDIODEVICEPARM_H

#include "portsysexmessage.h"

class RetSetAudioDeviceParm : public PortSysExMessage {
  public:
	RetSetAudioDeviceParm(Device *device);
	RetSetAudioDeviceParm(Command cmd, BYTE_VECTOR *message, Device *device)
		: PortSysExMessage(cmd, message, device) {}
	virtual ~RetSetAudioDeviceParm() override;

	void parseAnswerData() override;

	unsigned int getMaxControllers() const;

	bool getHostConnected() const;

	bool getPortReservedForVendorProduct() const;
	void setPortReservedForVendorProduct(bool getPortReservedForVendorProduct);

	bool hastHostName() { return m_iHostNameLength > 0; }
	std::string getHostName() const;

	UsbDeviceHost getUsbDeviceHost() const;

  private:
	AudioPortType m_AudioPortType = AudioPortType::APT_NONE;
	unsigned int m_iMaxControllers = 0;

	// USB Device & Host Parameters

	// USB Device Parameters
	UsbDeviceHost m_usbDeviceHost = UsbDeviceHost::NO_HOST;
	unsigned int m_iHostNameLength = 0;
	std::string m_sHostName;

	// USB Host Parameters
	bool m_bHostConnected = false;
	bool m_bPortReservedForVendorProduct = false;
};

#endif // RETSETAUDIODEVICEPARM_H
