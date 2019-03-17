#include "retsetaudiocontroldetailvalue.h"

RetSetAudioControlDetailValue::RetSetAudioControlDetailValue(Device *device)
	: PortSysExMessage(Command::RET_SET_AUDIO_CONTROL_DETAIL_VALUE,
					   SysExMessage::QUERY, device) {}

RetSetAudioControlDetailValue::~RetSetAudioControlDetailValue() {}

void RetSetAudioControlDetailValue::parseAnswerData() {}
