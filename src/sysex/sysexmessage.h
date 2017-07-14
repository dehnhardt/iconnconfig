#ifndef SYSEXMESSAGE_H
#define SYSEXMESSAGE_H

#include "../definitions.h"
#include "../device.h"
#include "midi.h"

#include <QObject>
#include <map>
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
		MANUFACTURER_NAME,	 /*!< Name of device manufacturer manufacturer */
		MODEL_NUMBER,		   /*!< Devices modelnumber */
		SERIAL_NUMBER,		   /*!< Devices serial number */
		FIRMWARE_VERSION,	  /*!< Installed firmware version */
		HARDWARE_VERSION,	  /*!< Hardware version of device */
		DEVICE_NAME = 0x10	 /*!< Name of device (writable) */
	};

	enum CommandFlags {
		ANSWER = 0x00, /*!< Answer from device */
		QUERY = 0x40   /*!< Query from device */
	};

	enum Errors {
		NO_DEVICE = -1,		  /*!< No device found or assigned */
		COMMAND_ACCEPTED = 0, /*!< No error */
		UNKNOWN_COMMAND,	  /*!< Command not known by device */
		MALFORMED_MESSAGE,	/*!< Message format has errors */
		COMMAND_FAILED		  /*!< Command failed for unknown reasin */
	};

	static CommandAcceptedAnswers commandAcceptedAnswers;

	// methods
public:
	SysExMessage(Command cmd, CommandFlags flags, Device *device);
	SysExMessage(Command cmd, BYTE_VECTOR *message, Device *device);

	virtual ~SysExMessage();
	virtual BYTE_VECTOR *getMIDISysExMessage();
	std::string getDataAsString();
	long getDataAsLong();
	Command parseAnswer(BYTE_VECTOR *answer);
	SysExMessage *getAnswer();
	SysExMessage *query();
	int execute();
	void setDebug(bool debug);
	void printRawData();
	virtual void parseAnswerData() {}

	// methods
	unsigned char getCmdflags() const;
	void setCmdflags(unsigned char value);

protected:
	virtual BYTE_VECTOR *getCommand() { return command; }
	virtual BYTE_VECTOR *getMessageData() { return new BYTE_VECTOR(); }
	virtual BYTE_VECTOR *getTransactionId() {
		if (transactionId == 0) {
			if (device != 0)
				return this->device->nextTransactionId();
			transactionId = new BYTE_VECTOR();
			transactionId->push_back(0x00);
			transactionId->push_back(0x01);
		}
		return transactionId;
	}
	bool checkAnswerValid(long answerCommandId);
	virtual void createAnswer(Command cmd __attribute__((unused)),
							  BYTE_VECTOR *message __attribute__((unused)),
							  Device *device __attribute__((unused)));
	void readSettings();
	void storeSettings();

	virtual int getSettingsId() = 0;
	virtual int getSettingsIndex() = 0;
	virtual std::string getStorableValue() = 0;

	// members
protected:
	Command cmd;
	unsigned char cmdflags;
	AcceptedAnswers acceptedAnswers;
	Device *device = 0;
	SysExMessage *answer;
	BYTE_VECTOR *command = 0;
	BYTE_VECTOR *transactionId = 0;
	BYTE_VECTOR *deviceHeader = 0;
	BYTE_VECTOR *resultData = 0;
	BYTE_VECTOR *data = 0;

	unsigned char commandVersionNumber = 0;

	bool debug = false;
	void extractData(std::vector<unsigned char> *message);
};

#endif// SYSEXMESSAGE_H
