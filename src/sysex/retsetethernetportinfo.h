#ifndef RETSETETHERNETPORTINFO_H
#define RETSETETHERNETPORTINFO_H

#include "sysexmessage.h"

class RetSetEthernetPortInfo : public SysExMessage {
public:
	RetSetEthernetPortInfo(Device *m_pDevice);

	RetSetEthernetPortInfo(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {
		m_pAddresses = new std::map<int, std::string>();
	}

	enum IPFlags {
		STATIC = 0x00,		/*!< IP address is static */
		DYNAMIC = 0x01,		/*!< IP address is dynamic */
		ADDRESS = 0x02,		/*!< ADRRESS */
		SUBNET_MASK = 0x04, /*!< Subnet mask */
		GATEWAY = 0x08		/*!< Gateway */
	};

public:
	void parseAnswerData();
	BYTE_VECTOR *m_pGetMessageData();
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

public:
	// getter
	IPFlags getMethod();
	std::string getAddress(int flags);
	std::string getMacAddress() const;
	std::string getBonjourName() const;

public:
	// setter
	void setMethod(IPFlags m_MethodFlag);
	void setAddress(int flags, std::string address);
	void setBonjourName(const std::string &value);

private:
	BYTE_VECTOR *m_pPortId = 0;
	std::string m_sMacAddress;
	std::string m_sBonjourName;
	IPFlags m_MethodFlag;
	std::map<int, std::string> *m_pAddresses = 0;
};

#endif// RETSETETHERNETPORTINFO_H
