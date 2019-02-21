#include "retgizmocount.h"

RetGizmoCount::RetGizmoCount(Device *device)
	: SysExMessage(Command::GET_GIZMO_COUNT, SysExMessage::QUERY, device) {}

void RetGizmoCount::parseAnswerData() {
	m_iGizmoCount = static_cast<unsigned int>(MIDI::byteJoin7bit(m_pData, 1, 2));
}
