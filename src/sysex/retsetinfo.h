#ifndef RETSETINFO_H
#define RETSETINFO_H

#include "sysexmessage.h"

#include <QObject>
#include <QtCore/QCoreApplication>

class RetSetInfo : public SysExMessage {
	Q_DECLARE_TR_FUNCTIONS(InfoItem)

public:
	// Constructors
	RetSetInfo(Device *m_pDevice);
	RetSetInfo(CommandFlags flags, Device *m_pDevice);
	RetSetInfo(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}

	// getter
	SysExMessage::DeviceInfoItem getInfoItem();
	std::string getValue();
	bool isItemEditable();
	std::string getItemName();

	// setter
	bool setValue(std::string m_sValue);

	// methods
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

protected:
	void parseAnswerData();
	BYTE_VECTOR *getMessageData();

private:
	// variables
	SysExMessage::DeviceInfoItem m_InfoItem;
	std::string m_sValue;
};

#endif// RETSETINFO_H
