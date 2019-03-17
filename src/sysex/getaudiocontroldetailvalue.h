#ifndef GETAUDIOCONTROLDETAILVALUE_H
#define GETAUDIOCONTROLDETAILVALUE_H

#include "portsysexmessage.h"

class GetAudioControlDetailValue : public PortSysExMessage {

  public:
	GetAudioControlDetailValue(Device *m_pDevice);
	virtual ~GetAudioControlDetailValue() override;

	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
					  Device *m_pDevice) override;

  protected:
	std::vector<unsigned char> *m_pGetMessageData() override;
};

#endif // GETAUDIOCONTROLDETAILVALUE_H
