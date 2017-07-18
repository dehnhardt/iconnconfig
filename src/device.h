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

class DeviceStructureContainer {

public:
	enum SetType { MESSAGE, STRUCTURE };
	SetType type;
	void setMessage(SysExMessage *message) {
		this->message = message;
		type = MESSAGE;
	}

	void setStructure(DeviceStructure *deviceStructure) {
		this->deviceStructure = deviceStructure;
		type = STRUCTURE;
	}

	SysExMessage *getMessage() { return message; }
	DeviceStructure *getStructure() { return deviceStructure; }

private:
	SysExMessage *message = 0;
	DeviceStructure *deviceStructure = 0;
};

class Device {
public:
	Device(unsigned int inPortNumber, unsigned int outPortNumber,
		   unsigned long serialNumber, unsigned int productId);
	Device(Device *device);

	~Device();

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
	BYTE_VECTOR *saveRestoreList = 0;

	bool loadConfigurationFromDevice();

	// getter
	bool getDebug() const;

	bool hasMidiSupport();

	static BYTE_VECTOR *getManufacturerHeader();
	BYTE_VECTOR *getDeviceHeader();
	BYTE_VECTOR *getFullHeader();
	bool queryDeviceInfo();
	bool isDeviceValid();

	std::string getModelName() { return modelName; }
	std::string getDeviceName() { return deviceName; }
	std::string getSerialNumberString() { return serialNumberString; }
	std::string getManufacturerName() { return manufacturerName; }
	std::string getFirmwareVersion() { return firmwareVersion; }
	std::string getHardwareVersion() { return hardwareVersion; }
	std::string getModelNumber() { return modelNumber; }
	unsigned int getInPortNumer() { return inPortNumber; }
	unsigned int getOutPortNumer() { return outPortNumber; }

	MIDISysexValue *getSerialNumber() { return serialNumber; }
	MIDISysexValue *getProductId() { return productId; }
	bool getDefault() { return isDefault; }
	RetCommandList *getCommands() { return commands; }
	GetInfo *getDeviceInfo() { return deviceInfo; }
	RetSetMidiInfo *getMidiInfo() { return midiInfo; }
	MIDI_PORT_INFOS *getMidiPortInfos() const;

	// setter
	void setDebug(bool value);
	void setDeviceInformation(std::string modelName, std::string deviceName);
	void setDefault(bool isDefault) { this->isDefault = isDefault; }

private:
	bool debug = false;

	unsigned int inPortNumber;
	unsigned int outPortNumber;

	unsigned int transactionId = 0;

	bool isDefault = false;

	RtMidiIn *midiin = 0;
	RtMidiOut *midiout = 0;

	MIDISysexValue *serialNumber;
	MIDISysexValue *productId;

	std::string modelName;
	std::string deviceName;
	std::string serialNumberString;
	std::string manufacturerName;
	std::string firmwareVersion;
	std::string hardwareVersion;
	std::string modelNumber;

	RetSetMidiInfo *midiInfo = 0;
	RetCommandList *commands = 0;
	RetInfoList *ii = 0;
	GetInfo *deviceInfo = 0;
	std::map<int, std::vector<RetSetMidiPortInfo *> *> *midiPortInfos = 0;

	DeviceStructure *informationTree = 0;

	BYTE_VECTOR *deviceHeader = 0;
	BYTE_VECTOR *fullHeader = 0;

	bool setupMidi();
	bool checkSysex(BYTE_VECTOR *data);
	void requestMidiPortInfos();
};

void midiOutErrorCallback(RtMidiError::Type type, const std::string &errorText,
						  void *userData);
void midiinErrorCallback(RtMidiError::Type type, const std::string &errorText,
						 void *userData);

#endif// DEVICE_H
