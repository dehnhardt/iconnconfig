#ifndef GETAUDIOPORTPARM_H
#define GETAUDIOPORTPARM_H

#include "../device.h"
#include "portsysexmessage.h"

class GetAudioPortParm : public PortSysExMessage {
  public:
	GetAudioPortParm(Device *device);
	~GetAudioPortParm() override;
	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
					  Device *m_pDevice) override;
	int getSettingsId() override { return m_Command; }
	int getSettingsIndex() override { return static_cast<int>(m_iPortId); }
	std::string getStorableValue() override { return ""; }

  protected:
	std::vector<unsigned char> *getMessageData() override;
};

#endif // GETAUDIOPORTPARM_H
