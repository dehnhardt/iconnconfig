#ifndef RETMIXERMETERVALUE_H
#define RETMIXERMETERVALUE_H

#include "portsysexmessage.h"

class RetMixerMeterValue : public PortSysExMessage {
  public:
	RetMixerMeterValue(Device *m_pDevice);
	RetMixerMeterValue(Command cmd, BYTE_VECTOR *message, Device *device)
		: PortSysExMessage(cmd, message, device) {}
	~RetMixerMeterValue();
	ChannelVolumes &getVolumes();

  protected:
	void parseAnswerData();

  private:
	ChannelVolumes m_vChannelVolumes;
	int m_iMixerOutputNumber;
	int m_iNumberOfAudioMeterBlocks;
};

#endif // RETMIXERMETERVALUE_H
