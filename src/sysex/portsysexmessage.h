#ifndef PORTSYSEXMESSAGE_H
#define PORTSYSEXMESSAGE_H

#include "sysexmessage.h"

class PortSysExMessage : public SysExMessage {
  public:
	PortSysExMessage(Command m_Command, CommandFlags flags, Device *m_pDevice);
	PortSysExMessage(Command m_Command, BYTE_VECTOR *message,
					 Device *m_pDevice);
	virtual ~PortSysExMessage();

	/*virtual void createAnswer(Command m_Command,
							  std::vector<unsigned char> *message,
							  Device *m_pDevice);*/
	int getSettingsIndex() { return static_cast<int>(m_iPortId); }

  public: // getter & setter
	unsigned int getPortNumber() { return m_iPortNumber; }
	unsigned int getPortId() { return m_iPortId; }
	unsigned int parsePortId(unsigned long offset = 1);
	void setPortNumber(unsigned int portNumber) { m_iPortNumber = portNumber; }
	void setPortId(unsigned int portId) { m_iPortId = portId; }
	BYTE_VECTOR *getPortIdBytes();

  protected:
	unsigned int m_iPortId = 0; /*!< Overall Portnumber for this section (MIDI
						  or Audio) and device */
	unsigned int m_iPortNumber = 0; /*!< Portnumber for a specific port type
							  (USB-Host, USB-Device, Network...) */
};

#endif // PORTSYSEXMESSAGE_H
