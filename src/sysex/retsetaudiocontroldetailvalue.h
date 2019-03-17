#ifndef RETSETAUDIOCONTROLDETAILVALUE_H
#define RETSETAUDIOCONTROLDETAILVALUE_H

#include "portsysexmessage.h"

class RetSetAudioControlDetailValue : public PortSysExMessage {
  public:
	RetSetAudioControlDetailValue(Device *m_pDevice);

	RetSetAudioControlDetailValue(Command cmd, BYTE_VECTOR *message,
								  Device *device)
		: PortSysExMessage(cmd, message, device) {}
	virtual ~RetSetAudioControlDetailValue() override;

	void parseAnswerData() override;
};

#endif // RETSETAUDIOCONTROLDETAILVALUE_H
