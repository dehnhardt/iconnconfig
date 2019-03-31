#include "retaudioportmetervalue.h"
#include <math.h>

RetAudioPortMeterValue::RetAudioPortMeterValue(Device *device)
	: PortSysExMessage(RET_AUDIO_PORT_METER_VALUE, SysExMessage::QUERY,
					   device) {
	// debug = true;
}

RetAudioPortMeterValue::~RetAudioPortMeterValue() {}

ChannelVolumes &RetAudioPortMeterValue::getVolumes() {
	return m_vChannelVolumes;
}

void RetAudioPortMeterValue::parseAnswerData() {
	unsigned long offset = 0;
	m_iCommandVersionNumber = m_pData->at(0);
	m_iPortId = static_cast<unsigned int>(MIDI::byteJoin7bit(m_pData, 1, 2));
	int numberOfAudioBlocks = m_pData->at(3);
	offset = 4;
	if (debug)
		std::cout << "Number of Audio Blocks: " << numberOfAudioBlocks
				  << std::endl;
	for (int i = 0; i < numberOfAudioBlocks; i++) {
		int direction = m_pData->at(offset) & 3;
		offset++;
		int numberOfValues = m_pData->at(offset);
		offset++;
		std::string dir;
		if (direction == 1) {
			m_vChannelVolumes.in.clear();
			if (debug)
				dir = "in ";
		}
		if (direction == 2) {
			m_vChannelVolumes.out.clear();
			if (debug)
				dir += "out ";
		}
		if (debug)
			std::cout << "Direction " << dir << " values follow: " << std::dec
					  << numberOfValues << std::endl;
		for (int y = 0; y < numberOfValues; y++) {
			int val = static_cast<int>(MIDI::byteJoin7bit(m_pData, offset, 2));
			offset += 2;
			ChannelVolume v;
			v.channel = y + 1;
			v.volume = val;
			// v.volume = db;
			switch (direction) {
			case 1:
				m_vChannelVolumes.in.push_back(v);
				break;
			case 2:
				m_vChannelVolumes.out.push_back(v);
				break;
			default:
				break;
			}
			if (debug)
				std::cout << "  - value for " << dir << " channel " << y
						  << " = " << val << std::endl;
		}
	}
}
