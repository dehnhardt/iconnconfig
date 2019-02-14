#ifndef DEVICE_H
#define DEVICE_H

#include "RtMidi.h"
#include "definitions.h"
#include "sysex/midi.h"

#include <map>
#include <string>
#include <vector>

#define MIDI_PORT_INFOS std::map<int, std::vector<RetSetMidiPortInfo *> *>

class SysExMessage;
class RetCommandList;
class RetInfoList;
class RetSetMidiInfo;
class RetSetMidiPortInfo;
class GetInfo;
class DeviceStructureContainer;

typedef std::map<unsigned int, DeviceStructureContainer *> DeviceStructure;

class DeviceStructureContainer
{

public:
	DeviceStructureContainer() {}
	DeviceStructureContainer(SysExMessage *message) { setMessage(message); }
	DeviceStructureContainer(DeviceStructure *structure)
	{
		setStructure(structure);
	}

	void setMessage(SysExMessage *message)
	{
		this->message = message;
		type = MESSAGE;
	}

	void setStructure(DeviceStructure *deviceStructure)
	{
		this->deviceStructure = deviceStructure;
		type = STRUCTURE;
	}

	SysExMessage *getMessage() { return message; }
	DeviceStructure *getStructure() { return deviceStructure; }

	enum SetType { MESSAGE, STRUCTURE };
	SetType type;

private:
	SysExMessage *message = nullptr;
	DeviceStructure *deviceStructure = nullptr;
};

class Device
{
public:
	Device(unsigned int m_iInPortNumber, unsigned int m_iOutPortNumber,
		   unsigned long m_pSerialNumber, unsigned int m_pProductId);
	Device(Device *device);

	~Device();

public:
	SysExMessage *getSysExMessage(Command cmd);

public:
	static const long MANUFACTURER_USB_ID = 0x2321;
	static const char MESSAGE_CLASS = 0x7e;
	static constexpr char MANUFACTURER_SYSEX_ID[3] = {0x00, 0x01, 0x73};
	static BYTE_VECTOR *manufacturerHeader;

	static const int DATA_LENGTH_OFFSET = 16;
	static const int DATA_LENGTH_LENGTH = 2;
	static const int DATA_OFFSET = 18;

	// methods
	void sentSysex(BYTE_VECTOR *data);
	void disconnect();
	void connect();

	BYTE_VECTOR *retrieveSysex();
	BYTE_VECTOR *nextTransactionId();
	BYTE_VECTOR *saveRestoreList = nullptr;

	bool loadConfigurationFromDevice();

	// getter
	bool getDebug() const;

	bool hasMidiSupport();

	static BYTE_VECTOR *getManufacturerHeader();
	BYTE_VECTOR *getDeviceHeader();
	BYTE_VECTOR *getFullHeader();
	bool queryDeviceInfo();
	bool isDeviceValid();

	std::string getModelName() { return m_sModelName; }
	std::string getDeviceName() { return m_sDeviceName; }
	std::string getSerialNumberString() { return m_sSerialNumber; }
	std::string getManufacturerName() { return m_sManufacturerName; }
	std::string getFirmwareVersion() { return m_sFirmwareVersion; }
	std::string getHardwareVersion() { return m_sHardwareVersion; }
	std::string getModelNumber() { return m_sModelNumber; }
	unsigned int getInPortNumer() { return m_iInPortNumber; }
	unsigned int getOutPortNumer() { return m_iOutPortNumber; }

	MIDISysexValue *getSerialNumber() { return m_pSerialNumber; }
	MIDISysexValue *getProductId() { return m_pProductId; }
	bool getDefault() { return m_bIsDefault; }
	RetCommandList *getCommands() { return m_pCommands; }
	GetInfo *getDeviceInfo() { return m_pDeviceInfo; }
	RetSetMidiInfo *getMidiInfo() { return m_pMidiInfo; }
	MIDI_PORT_INFOS *getMidiPortInfos() const;
	BYTE_VECTOR *getLastSendMessage() const;
	BYTE_VECTOR *getLastRetrieveMessage() const;

	// setter
	void setDebug(bool value);
	void setDeviceInformation(std::string m_sModelName, std::string m_sDeviceName);
	void setDefault(bool isDefault) { this->m_bIsDefault = isDefault; }

	void setLastSendMessage(BYTE_VECTOR *value);
	void setLastRetrieveMessage(BYTE_VECTOR *value);

private:
	bool setupMidi();
	bool checkSysex(BYTE_VECTOR *data);
	void requestMidiPortInfos();
	void addCommandToStructure(Command cmd,
							   DeviceStructureContainer *structureContainer);

	bool debug = false;

	unsigned int m_iInPortNumber;
	unsigned int m_iOutPortNumber;

	unsigned int m_iTransactionId = 0;

	BYTE_VECTOR *m_pLastSendMessage = nullptr;
	BYTE_VECTOR *m_pLastRetrieveMessage = nullptr;

	bool m_bIsDefault = false;

	RtMidiIn *midiin = nullptr;
	RtMidiOut *midiout = nullptr;

	MIDISysexValue *m_pSerialNumber;
	MIDISysexValue *m_pProductId;

	std::string m_sModelName;
	std::string m_sDeviceName;
	std::string m_sSerialNumber;
	std::string m_sManufacturerName;
	std::string m_sFirmwareVersion;
	std::string m_sHardwareVersion;
	std::string m_sModelNumber;

	RetSetMidiInfo *m_pMidiInfo = nullptr;
	RetCommandList *m_pCommands = nullptr;
	RetInfoList *m_pRetInfoList = nullptr;
	GetInfo *m_pDeviceInfo = nullptr;
	std::map<int, std::vector<RetSetMidiPortInfo *> *> *m_pMidiPortInfos = nullptr;

	DeviceStructure *m_pInformationTree = nullptr;

	BYTE_VECTOR *m_pDeviceHeader = nullptr;
	BYTE_VECTOR *m_pFullHeader = nullptr;

};

void midiOutErrorCallback(RtMidiError::Type type, const std::string &errorText,
						  void *userData);
void midiinErrorCallback(RtMidiError::Type type, const std::string &errorText,
						 void *userData);

#endif// DEVICE_H
