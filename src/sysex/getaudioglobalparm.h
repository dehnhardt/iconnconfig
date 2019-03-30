#ifndef GETAUDIOGLOBALPARM_H
#define GETAUDIOGLOBALPARM_H

#include "retsetaudioglobalparm.h"
#include "sysexmessage.h"

class GetAudioGlobalParm : public SysExMessage {
  public:
	GetAudioGlobalParm(Device *device);
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

  private:
	void createAnswer(Command cmd, BYTE_VECTOR *message, Device *device) {
		m_pAnswer =
			std::make_shared<RetSetAudioGlobalParm>(cmd, message, device);
		m_pAnswer->parseAnswerData();
	}
};

#endif // GETAUDIOGLOBALPARM_H
