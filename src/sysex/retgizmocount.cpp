#include "retgizmocount.h"

RetGizmoCount::RetGizmoCount(Device *device)
	: SysExMessage(Command::GET_GIZMO_COUNT, SysExMessage::QUERY, device) {}

void RetGizmoCount::parseAnswerData() {
	gizmoCount = static_cast<unsigned int>(MIDI::byteJoin(data, 1, 2));
}
