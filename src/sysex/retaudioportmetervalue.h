#ifndef RETAUDIOPORTMETERVALUE_H
#define RETAUDIOPORTMETERVALUE_H

#include "portsysexmessage.h"

class RetAudioPortMeterValue : public PortSysExMessage {
  public:
	RetAudioPortMeterValue(Device *m_pDevice);
	RetAudioPortMeterValue(Command cmd, BYTE_VECTOR *message, Device *device)
		: PortSysExMessage(cmd, message, device) {}
	~RetAudioPortMeterValue();
	ChannelVolumes &getVolumes();

  protected:
	void parseAnswerData();

  private:
	ChannelVolumes m_vChannelVolumes;
};

#endif // RETAUDIOPORTMETERVALUE_H
