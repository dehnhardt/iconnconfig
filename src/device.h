#ifndef DEVICE_H
#define DEVICE_H

#include "RtMidi.h"
#include "definitions.h"
#include "sysex/midi.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

class SysExMessage;
class RetCommandList;
class RetInfoList;
class RetSetMidiInfo;
class RetSetMidiPortInfo;
class GetInfo;
class DeviceStructureContainer;
class RetSetAudioGlobalParm;
class RetSetAudioPortParm;
class RetSetAudioChannelName;
class RetSetMixerInputParm;
class RetSetMixerOutputParm;
class RetSetMixerPortParm;

typedef std::map<unsigned int, std::shared_ptr<DeviceStructureContainer>>
	DeviceStructure;
typedef std::map<int, std::vector<std::shared_ptr<RetSetMidiPortInfo>> *>
	MidiPortInfos;
typedef std::map<AudioPortType,
				 std::vector<std::shared_ptr<RetSetAudioPortParm>> *>
	AudioPortStructure;

typedef std::map<AudioChannelId, std::shared_ptr<RetSetAudioChannelName>>
	AudioChannelNames;
typedef std::map<ChannelDirection, AudioChannelNames> AudioDirectionChannels;
typedef std::map<AudioPortId, AudioDirectionChannels> AudioChannelStructure;
typedef std::map<AudioPortChannelId, std::shared_ptr<RetSetMixerInputParm>>
	AudioMixerInputChannels;
typedef std::map<AudioPortChannelId, std::shared_ptr<RetSetMixerOutputParm>>
	AudioMixerOutputChannels;

class DeviceStructureContainer {

  public:
	DeviceStructureContainer() {}
	DeviceStructureContainer(std::shared_ptr<SysExMessage> message) {
		setMessage(message);
	}
	DeviceStructureContainer(std::shared_ptr<DeviceStructure> structure) {
		setStructure(structure);
	}

	void setMessage(std::shared_ptr<SysExMessage> message) {
		this->message = message;
		type = MESSAGE;
	}

	void setStructure(std::shared_ptr<DeviceStructure> deviceStructure) {
		this->deviceStructure = deviceStructure;
		type = STRUCTURE;
	}

	std::shared_ptr<SysExMessage> getMessage() { return message; }
	std::shared_ptr<DeviceStructure> getStructure() { return deviceStructure; }

	enum SetType { MESSAGE, STRUCTURE };
	SetType type;

  private:
	std::shared_ptr<SysExMessage> message;
	std::shared_ptr<DeviceStructure> deviceStructure;
};

class Device {
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

	// static device specific methods
	static double intToDecibel(int val) { return val / 256.0; }

	// methods
	void sentSysex(BYTE_VECTOR *data);
	void disconnect();
	void connect();

	BYTE_VECTOR *retrieveSysex();
	BYTE_VECTOR *nextTransactionId();
	BYTE_VECTOR *m_pSaveRestoreList = nullptr;

	bool loadConfigurationFromDevice();

	// global getters
	bool getDebug() const;

	bool hasMidiSupport();
	bool hasAudioSupport();

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
	std::shared_ptr<RetCommandList> getCommands() { return m_pCommands; }
	std::shared_ptr<GetInfo> getDeviceInfo() { return m_pDeviceInfo; }
	std::shared_ptr<RetSetMidiInfo> getMidiInfo() { return m_pMidiInfo; }
	std::shared_ptr<RetSetAudioGlobalParm> getAudioGlobalParm() {
		return m_pGlobalAudioParam;
	}
	MidiPortInfos *getMidiPortInfos() const;
	std::shared_ptr<AudioPortStructure> getAudioPortStructure();

	std::vector<std::shared_ptr<RetSetAudioChannelName>> m_vInputChannels;
	std::vector<std::shared_ptr<RetSetAudioChannelName>> m_vOutputChannels;

	BYTE_VECTOR *getLastSendMessage() const;
	BYTE_VECTOR *getLastRetrieveMessage() const;

	// global setters
	void setDebug(bool value);
	void setDeviceInformation(std::string m_sModelName,
							  std::string m_sDeviceName);
	void setDefault(bool isDefault) { this->m_bIsDefault = isDefault; }

	void setLastSendMessage(BYTE_VECTOR *value);
	void setLastRetrieveMessage(BYTE_VECTOR *value);

	// audio getters
	std::shared_ptr<RetSetAudioGlobalParm> getGlobalAudioParam() const;

	std::shared_ptr<AudioChannelStructure> getAudioChannelStructure();
	AudioDirectionChannels getAudioDirectionChannels(AudioPortId audioPortId);
	AudioChannelNames getAudioChannelNames(AudioPortId audioPortId,
										   ChannelDirection channelDirection);

	std::vector<std::shared_ptr<RetSetAudioChannelName>> getInputChannels();
	std::vector<std::shared_ptr<RetSetAudioChannelName>> getOutputChannels();

	std::map<AudioPortId, std::shared_ptr<RetSetAudioPortParm>>
	getAudioPorts() const;
	std::shared_ptr<RetSetAudioPortParm> getAudioPort(AudioPortId audioPortId);

	std::shared_ptr<RetSetMixerPortParm> getMixerPortParm();
	std::shared_ptr<AudioMixerInputChannels> getAudioMixerInputChannels();
	std::shared_ptr<AudioMixerOutputChannels> getAudioMixerOutputChannels();

  private:
	// Midi methods
	bool setupMidi();
	bool checkSysex(BYTE_VECTOR *data);
	void requestMidiPortInfos();
	void addCommandToStructure(Command cmd,
							   DeviceStructureContainer *structureContainer);

	// Audio methods
	void queryAudioPorts();
	AudioChannelNames queryAudioChannels(unsigned int portId,
										 ChannelDirection direction,
										 unsigned int numberOfChannels);
	void queryAudioMixerChannels(ChannelDirection channelDirection);
	void queryMixerPortParm();

	// Variables

	// Device variables
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

	// MIDI structures
	std::shared_ptr<RetSetMidiInfo> m_pMidiInfo;
	MidiPortInfos *m_pMidiPortInfos = nullptr;

	// Audio structures
	std::shared_ptr<RetSetAudioGlobalParm> m_pGlobalAudioParam = nullptr;
	std::shared_ptr<AudioPortStructure> m_pAudioPortParms = nullptr;
	std::shared_ptr<AudioChannelStructure> m_pAudioChannelStructure = nullptr;
	std::map<AudioPortId, std::shared_ptr<RetSetAudioPortParm>> m_mAudioPorts;
	std::shared_ptr<RetSetMixerPortParm> m_pRetSetMixerPortParm = nullptr;
	std::shared_ptr<AudioMixerInputChannels> m_pAudioMixerInputChannels =
		nullptr;
	std::shared_ptr<AudioMixerOutputChannels> m_pAudioMixerOutputChannels =
		nullptr;

	// Global device / command variables
	std::shared_ptr<RetCommandList> m_pCommands;
	std::shared_ptr<RetInfoList> m_pRetInfoList;
	std::shared_ptr<GetInfo> m_pDeviceInfo;

	DeviceStructure *m_pInformationTree = nullptr;

	BYTE_VECTOR *m_pDeviceHeader = nullptr;
	BYTE_VECTOR *m_pFullHeader = nullptr;
};

void midiOutErrorCallback(RtMidiError::Type type, const std::string &errorText,
						  void *userData);
void midiinErrorCallback(RtMidiError::Type type, const std::string &errorText,
						 void *userData);

#endif // DEVICE_H
