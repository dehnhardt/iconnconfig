#ifndef RETSETMIDIPORTROUTE_H
#define RETSETMIDIPORTROUTE_H

#include "sysexmessage.h"

class RetSetMidiPortRoute : public SysExMessage {
  public:
	RetSetMidiPortRoute(Device *m_pDevice);

	RetSetMidiPortRoute(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}

	~RetSetMidiPortRoute();

	void parseAnswerData();
	bool isPortRouted(int portNumber);
	void setPortRouted(int portNumber, bool routed);

	int getTotalNumberOfPorts() const;
	void setTotalNumberOfPorts(int value);
	BYTE_VECTOR *getMessageData();

	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

  private:
	int getNumberOfExpectedBytes(int totalPortNumber);
	void getPortByteAndBit(int portNumber, int &byte, int &bit);

	long m_iPortId;
	int m_iTotalNumberOfPorts = -1;
	int m_iNumerOfExpectedBytes = -1;
	BYTE_VECTOR *m_pPortRoutings = nullptr;
};

#endif // RETSETMIDIPORTROUTE_H
