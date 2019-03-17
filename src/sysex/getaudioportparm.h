#ifndef GETAUDIOPORTPARM_H
#define GETAUDIOPORTPARM_H

#include "../device.h"
#include "sysexmessage.h"

class GetAudioPortParm : public SysExMessage {
  public:
	GetAudioPortParm(Device *device);
	~GetAudioPortParm() override;
	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
					  Device *m_pDevice) override;
	int getSettingsId() override { return m_Command; }
	int getSettingsIndex() override { return static_cast<int>(m_iPortId); }
	std::string getStorableValue() override { return ""; }

  public:
	// setter
	void setPortId(unsigned int portId) { this->m_iPortId = portId; }

  protected:
	std::vector<unsigned char> *m_pGetMessageData() override;

  private:
	unsigned int m_iPortId;
};

#endif // GETAUDIOPORTPARM_H
