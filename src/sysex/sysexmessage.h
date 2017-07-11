#ifndef SYSEXMESSAGE_H
#define SYSEXMESSAGE_H

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
	/**
	* @brief The Command enum
	*/
	enum Command {
		CMD_ERROR = -1, /*!< Return if command is not valid */
		/* device commands */
		GET_DEVICE = 0x01, /*!< Query for devices  */
		RET_DEVICE,		   /*!< Answer from device to GET_DEVICE query*/
		GET_COMMAND_LIST,  /*!< Request devices command list. */
		RET_COMMAND_LIST,  /*!< List of commands supported by device. */
		GET_INFO_LIST,	 /*!< Request list of infos, whch can be retrieved */
		RET_INFO_LIST,	 /*!< List of infos supported by this device */
		GET_INFO,		   /*!< Get a single information from device */
		RET_SET_INFO,	  /*!< Return a single information from device */
		GET_RESET_LIST,	/*!< Request list of reset IDs */
		RET_RESET_LIST,	/*!< Return list of reset IDs */
		GET_SAVE_RESTORE_LIST,  /*!< Request the save / restore ids of */
		RET_SAVE_RESTORE_LIST,  /*!< Returns a list of the save/restore ids */
		GET_ETHERNET_PORT_INFO, /*!< Requests the information about a specific
								   ethernet port */
		RET_SET_ETHERNET_PORT_INFO, /*!< Returns the information about a
									   specific ethernet port or sets some
									   values for a specific ethernet port */
		ACK,		  /*!< Returns a sucess code (0x00) or an error code (0x01 -
					 0x03) */
		RESET,		  /*!< Resets the device */
		SAVE_RESTORE, /*!< Used to save or restore the current device
						 configuration */
		GET_GIZMO_COUNT, /*!< Number of other devices connected to this device
							*/
		RET_GIZMO_COUNT, /*!< Returns the number of devices that are
						connected to this device */
		GET_GIZMO_INFO,  /*!< Queueries information about a connected
						device */
		RET_GIZMO_INFO,  /*!< Returns information about a connected
						device */
		/* MIDI Commands */
		GET_MIDI_INFO =
			0x20, /*!< Queueries information about the midi parameters */
		RET_SET_MIDI_INFO, /*!< Returns information about the midi parameters */
		GET_MIDI_PORT_INFO, /*!< Queuery the information about a specific MID
							   Port */
		RET_SET_MIDI_PORT_INFO, /*!< Returns or set the MIDI-Information
									 for a specific port */
		GET_MIDI_PORT_FILTER, /*!< Queueries infomation about current midi port
								filters for a specific port */
		RET_SET_MIDI_PORT_FILTER, /*!< Returns or writes infomation about
									 current midi port filters for a specific
									 port*/
		GET_MIDI_PORT_REMAP,
		RET_SET_MIDI_PORT_REMAP,
		GET_MIDI_PORT_ROUTE,
		RET_SET_MIDI_PORT_ROUTE
	};

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
	virtual void createAnswer(SysExMessage::Command cmd __attribute__((unused)),
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
