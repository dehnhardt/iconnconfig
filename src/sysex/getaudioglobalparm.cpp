#include "getaudioglobalparm.h"

GetAudioGlobalParm::GetAudioGlobalParm(Device *device)
	: SysExMessage(Command::GET_AUDIO_GLOBAL_PARM, SysExMessage::QUERY,
				   device) {}
