#ifndef SYSEXMESSAGE_H
#define SYSEXMESSAGE_H

#include "../definitions.h"
#include "../device.h"
#include "midi.h"

#include <map>
#include <memory>
#include <vector>

typedef std::vector<int> AcceptedAnswers;
typedef std::map<int, AcceptedAnswers> CommandAcceptedAnswers;

/**
 * @brief The SysExMessage class
 */
class SysExMessage {

	// enums
  public:
	enum DeviceInfoItem {
		ACCESSORY_NAME = 0x01, /*!< Name of acessory */
		MANUFACTURER_NAME,     /*!< Name of device manufacturer manufacturer */
		MODEL_NUMBER,          /*!< Devices modelnumber */
		SERIAL_NUMBER,         /*!< Devices serial number */
		FIRMWARE_VERSION,      /*!< Installed firmware version */
		HARDWARE_VERSION,      /*!< Hardware version of device */
		DEVICE_NAME = 0x10     /*!< Name of device (writable) */
	};

	enum CommandFlags {
		ANSWER = 0x00, /*!< Answer from device */
		QUERY = 0x40   /*!< Query from device */
	};

	enum Errors {
		NO_DEVICE = -1,       /*!< No device found or assigned */
		COMMAND_ACCEPTED = 0, /*!< No error */
		UNKNOWN_COMMAND,      /*!< Command not known by device */
		MALFORMED_MESSAGE,    /*!< Message format has errors */
		COMMAND_FAILED        /*!< Command failed for unknown reasin */
	};

	static CommandAcceptedAnswers commandAcceptedAnswers;

  public:
	SysExMessage(Command m_Command, CommandFlags flags, Device *m_pDevice);
	SysExMessage(Command m_Command, BYTE_VECTOR *message, Device *m_pDevice);
	virtual ~SysExMessage();

	// methods
	virtual BYTE_VECTOR *getMIDISysExMessage();
	std::string getDataAsString();
	long getDataAsLong();
	Command parseAnswer(BYTE_VECTOR *m_pAnswer);
	SysExMessage *getAnswer();
	std::shared_ptr<SysExMessage> getAnswerSmart();
	SysExMessage *query();
	std::shared_ptr<SysExMessage> querySmart();
	int execute();
	void setDebug(bool debug);
	bool getDebug();
	void printRawData();
	unsigned int getCommandNumber();
	bool isWriteCommand();
	virtual void parseAnswerData() {}

	// getter
	unsigned char getCmdflags() const;
	Command getCommand() { return m_Command; }
	Device *getDevice() const;

	// setter
	void setCmdflags(unsigned char value);

	// helper
	void printMessage();

  protected:
	virtual BYTE_VECTOR *getCommandData() { return m_pCommandData; }
	virtual BYTE_VECTOR *getMessageData() { return new BYTE_VECTOR(); }
	virtual BYTE_VECTOR *getTransactionId() {
		if (m_pTransactionId == nullptr) {
			if (m_pDevice != nullptr)
				return this->m_pDevice->nextTransactionId();
			m_pTransactionId = new BYTE_VECTOR();
			m_pTransactionId->push_back(0x00);
			m_pTransactionId->push_back(0x01);
		}
		return m_pTransactionId;
	}
	bool checkAnswerValid(long answerCommandId);
	virtual void createAnswer(Command m_Command __attribute__((unused)),
							  BYTE_VECTOR *message __attribute__((unused)),
							  Device *m_pDevice __attribute__((unused)));
	virtual void createAck(BYTE_VECTOR *message, Device *m_pDevice);
	void readSettings();
	void storeSettings();

	virtual int getSettingsId() { return m_Command; }
	virtual int getSettingsIndex() { return 0; }
	virtual std::string getStorableValue() {
		return std::string(m_pData->begin(), m_pData->end());
	}

	void extractData(std::vector<unsigned char> *message);

	// members
  protected:
	Command m_Command;
	unsigned int m_iDataLength = 0;
	unsigned char m_iCmdflags;
	AcceptedAnswers m_AcceptedAnswers;
	Device *m_pDevice;
	std::shared_ptr<SysExMessage> m_pAnswer;
	BYTE_VECTOR *m_pCommandData = nullptr;
	BYTE_VECTOR *m_pTransactionId = nullptr;
	BYTE_VECTOR *m_pDeviceHeader = nullptr;
	BYTE_VECTOR *m_pResultData = nullptr;
	std::shared_ptr<BYTE_VECTOR> m_pData = nullptr;

	unsigned char m_iCommandVersionNumber = 0;

	bool debug = false;
};

#endif // SYSEXMESSAGE_H
